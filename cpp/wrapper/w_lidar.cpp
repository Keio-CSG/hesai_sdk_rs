#include "w_lidar.h"


std::unique_ptr<DriverParam> createDriverParam(std::string pcap_path, std::string correction_file_path, std::string firetimes_path) {
    auto param = std::make_unique<DriverParam>();
    param->input_param.source_type = DATA_FROM_PCAP;
    param->input_param.pcap_path = pcap_path;
    param->input_param.correction_file_path = correction_file_path;
    param->input_param.firetimes_path = firetimes_path;

    param->input_param.device_ip_address = "";
    param->input_param.ptc_port = 0;
    param->input_param.udp_port = 0;
    param->input_param.host_ip_address = "";
    param->input_param.multicast_ip_address = "";

    return param;
}


int WLidar::Init(const std::unique_ptr<DriverParam> param) {
    return lidar_ptr_->Init(*param);
}

void WLidar::Finish() {
    delete lidar_ptr_;
}

std::unique_ptr<WLidarDecodedFrame> WLidar::Consume(uint16_t& next_packet_index, bool& is_finished) {
    UdpPacket packet;
    FaultMessageInfo fault_message_info;
    int ret = lidar_ptr_->GetOnePacket(packet);
    if (ret == -1) {
        is_finished = true;
        return nullptr;
    }

    if (packet.packet_len == kFaultMessageLength) {
        // FaultMessageCallback(packet, fault_message_info);
        return nullptr;
    }

    LidarDecodedPacket<LidarPointXYZIRT> decoded_packet;
    lidar_ptr_->DecodePacket(decoded_packet, packet);

    std::unique_ptr<WLidarDecodedFrame> out_frame;
    if (decoded_packet.scan_complete) {
        int waiting_packet_index = next_packet_index > 1 ? next_packet_index - 1 : 0;
        while (!lidar_ptr_->ComputeXYZIComplete(waiting_packet_index)) {
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
        // uint32_t end = GetMicroTickCount();
        if (lidar_ptr_->frame_.points_num > kMinPointsOfOneFrame) {
            out_frame = std::make_unique<WLidarDecodedFrame>();
            out_frame->copy_from(lidar_ptr_->frame_);
        }

        lidar_ptr_->frame_.Update();

        next_packet_index = 0;

        if (decoded_packet.IsDecodedPacketValid()) {
            decoded_packet.packet_index = next_packet_index;
            lidar_ptr_->ComputeXYZI(decoded_packet);
            next_packet_index++;
        }
    }
    else {
        decoded_packet.packet_index = next_packet_index;
        lidar_ptr_->ComputeXYZI(decoded_packet);
        next_packet_index++;

        if (next_packet_index >= kMaxPacketNumPerFrame) {
            next_packet_index = 0;
            lidar_ptr_->frame_.Update();
            printf("fail to start a new frame\n");
        }
    }
    return out_frame;
}
