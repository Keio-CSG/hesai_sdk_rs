#ifndef W_LIDAR_H
#define W_LIDAR_H

#include <memory>
#include "lidar.h"
#include "w_lidar_types.h"
#include "lidar_types.h"
#include "fault_message.h"

std::unique_ptr<DriverParam> createDriverParam(std::string pcap_path, std::string correction_file_path, std::string firetimes_path);

class WLidar {
public:
    hesai::lidar::Lidar<LidarPointXYZIRT> *lidar_ptr_;
    ~WLidar() {}
    WLidar() {
        lidar_ptr_ = new hesai::lidar::Lidar<LidarPointXYZIRT>;
    }

    int Init(const std::unique_ptr<DriverParam> param);

    void Finish();

    // int GetGeneralParser(GeneralParser<LidarPointXYZIRT> **parser) {
    //     return lidar_ptr_->GetGeneralParser(parser);
    // }

    // int SetLidarType(std::string lidar_type) {
    //     return lidar_ptr_->SetLidarType(lidar_type);
    // }

    // std::unique_ptr<UdpPacket> GetOnePacket() {
    //     std::unique_ptr<UdpPacket> packet = std::make_unique<UdpPacket>();
    //     lidar_ptr_->GetOnePacket(*packet);
    //     return packet;
    // }

    // int StartRecordPcap(std::string record_path) {
    //     return lidar_ptr_->StartRecordPcap(record_path);
    // }

    // int StopRecordPcap() {
    //     return lidar_ptr_->StopRecordPcap();
    // }

    // int SaveUdpPacket(const std::string &record_path, const UdpFrame_t &packets, int port = 2368) {
    //     return lidar_ptr_->SaveUdpPacket(record_path, packets, port);
    // }

    // int SaveUdpPacket(const std::string &record_path, const UdpFrameArray_t &packets, int port = 2368) {
    //     return lidar_ptr_->SaveUdpPacket(record_path, packets, port);
    // }

    // int ComputeXYZI(std::unique_ptr<WLidarDecodedPacket> packet) {
    //     return lidar_ptr_->ComputeXYZI(*packet);
    // }

    // WLidarDecodedPacket DecodePacketToPacket(const UdpPacket &udp_packet) {
    //     LidarDecodedPacket<LidarPointXYZIRT> output;
    //     lidar_ptr_->DecodePacket(output, udp_packet);
    //     WLidarDecodedPacket w_frame;
    //     w_frame.copy_from(output);
    //     return w_frame;
    // }

    // WLidarDecodedFrame DecodePacketToFrame(const UdpPacket &udp_packet) {
    //     LidarDecodedFrame<LidarPointXYZIRT> frame;
    //     lidar_ptr_->DecodePacket(frame, udp_packet);
    //     WLidarDecodedFrame w_frame;
    //     w_frame.copy_from(frame);
    //     return w_frame;
    // }

    // bool ComputeXYZIComplete(int index) {
    //     return lidar_ptr_->ComputeXYZIComplete(index);
    // }

    // int SaveCorrectionFile(std::string correction_save_path) {
    //     return lidar_ptr_->SaveCorrectionFile(correction_save_path);
    // }

    // int LoadCorrectionForUdpParser() {
    //     return lidar_ptr_->LoadCorrectionForUdpParser();
    // }

    // void LoadCorrectionFile(std::string correction_path) {
    //     lidar_ptr_->LoadCorrectionFile(correction_path);
    // }

    // int LoadCorrectionString(char *correction_string) {
    //     return lidar_ptr_->LoadCorrectionString(correction_string);
    // }

    // void LoadFiretimesFile(std::string firetimes_path) {
    //     lidar_ptr_->LoadFiretimesFile(firetimes_path);
    // }

    // void SetUdpParser(UdpParser<LidarPointXYZIRT> *udp_parser) {
    //     lidar_ptr_->SetUdpParser(udp_parser);
    // }

    // UdpParser<LidarPointXYZIRT> *GetUdpParser() {
    //     return lidar_ptr_->GetUdpParser();
    // }

    // void EnableRecordPcap(bool bRecord) {
    //     lidar_ptr_->EnableRecordPcap(bRecord);
    // }

    // void SetThreadNum(int thread_num) {
    //     lidar_ptr_->SetThreadNum(thread_num);
    // }

    // void SetSource(Source **source) {
    //     lidar_ptr_->SetSource(source);
    // }

    // std::string GetLidarType() {
    //     return lidar_ptr_->GetLidarType();
    // }

    // WLidarDecodedFrame* GetFrame() {

    // }

    std::unique_ptr<WLidarDecodedFrame> Consume(uint16_t& packet_index, bool& is_finished);
};

#endif