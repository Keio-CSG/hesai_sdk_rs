#ifndef W_LIDAR_TYPES_H
#define W_LIDAR_TYPES_H
#include <stdint.h>
#include "lidar_types.h"

struct WLidarDecodedPacket {
    uint64_t host_timestamp;
    uint64_t sensor_timestamp;
    float duration;
    double distance_unit;
    uint32_t maxPoints;
    uint32_t points_num;
    uint16_t block_num;
    uint16_t laser_num;
    int packet_index;
    bool scan_complete;    // when this packet is the last packet in one frame, this value should be true
    uint8_t reflectivities[hesai::lidar::kMaxPointsNumPerPacket];
    uint16_t distances[hesai::lidar::kMaxPointsNumPerPacket];
    float azimuth[hesai::lidar::kMaxPointsNumPerPacket];
    float elevation[hesai::lidar::kMaxPointsNumPerPacket];
    uint16_t azimuths;
    uint16_t spin_speed;
    uint8_t lidar_state;
    uint8_t work_mode;
    bool IsDecodedPacketValid() const {
        return block_num != 0;
    }

    void copy_from(hesai::lidar::LidarDecodedPacket<hesai::lidar::LidarPointXYZIRT> p) {
        host_timestamp = p.host_timestamp;
        sensor_timestamp = p.sensor_timestamp;
        duration = p.duration;
        distance_unit = p.distance_unit;
        maxPoints = p.maxPoints;
        points_num = p.points_num;
        block_num = p.block_num;
        laser_num = p.laser_num;
        packet_index = p.packet_index;
        scan_complete = p.scan_complete;
        azimuths = p.azimuths;
        spin_speed = p.spin_speed;
        lidar_state = p.lidar_state;
        work_mode = p.work_mode;
        for (int i = 0; i < p.points_num; i++) {
            azimuth[i] = p.azimuth[i];
            elevation[i] = p.elevation[i];
            distances[i] = p.distances[i];
            reflectivities[i] = p.reflectivities[i];
        }
    }
};

class WLidarDecodedFrame {
public:
    WLidarDecodedFrame() {
        points_num = 0;
        packet_index = 0;
        distance_unit = 0.0;
        // points = new hesai::lidar::LidarPointXYZIRT[
        // hesai::lidar::kMaxPacketNumPerFrame *
        // hesai::lidar::kMaxPointsNumPerPacket];
        // sensor_timestamp = new uint64_t[hesai::lidar::kMaxPacketNumPerFrame];
        // azimuths = new uint16_t[hesai::lidar::kMaxPacketNumPerFrame];
        // azimuth = new float[
        // hesai::lidar::kMaxPacketNumPerFrame *
        // hesai::lidar::kMaxPointsNumPerPacket];
        // elevation = new float[
        // hesai::lidar::kMaxPacketNumPerFrame *
        // hesai::lidar::kMaxPointsNumPerPacket];
        // distances = new uint16_t[
        // hesai::lidar::kMaxPacketNumPerFrame *
        // hesai::lidar::kMaxPointsNumPerPacket];
        // reflectivities = new uint8_t[
        // hesai::lidar::kMaxPacketNumPerFrame *
        // hesai::lidar::kMaxPointsNumPerPacket];
        host_timestamp = 0;
        major_version = 0;
        minor_version = 0;
        return_mode = 0;
        spin_speed = 0;
        points_num = 0;
        packet_num = 0;
        block_num = 0;
        laser_num = 0;
        packet_index = 0;
        scan_complete = false;
        distance_unit = 0;
        frame_index = 0;
    };

    void copy_from(hesai::lidar::LidarDecodedFrame<hesai::lidar::LidarPointXYZIRT>& f) {
        host_timestamp = f.host_timestamp;
        major_version = f.major_version;
        minor_version = f.minor_version;
        return_mode = f.return_mode;
        spin_speed = f.spin_speed;
        points_num = f.points_num;
        packet_num = f.packet_num;
        block_num = f.block_num;
        laser_num = f.laser_num;
        packet_index = f.packet_index;
        scan_complete = f.scan_complete;
        distance_unit = f.distance_unit;
        lidar_state = f.lidar_state;
        work_mode = f.work_mode;
        frame_index = f.frame_index;
        // points = std::vector<hesai::lidar::LidarPointXYZIRT>(f.points_num);
        // azimuth = std::vector<float>(f.points_num);
        // elevation = std::vector<float>(f.points_num);
        // distances = std::vector<uint16_t>(f.points_num);
        // reflectivities = std::vector<uint8_t>(f.points_num);
        // sensor_timestamp = std::vector<uint64_t>(f.packet_num);
        // azimuths = std::vector<uint16_t>(f.packet_num);
        for (int i = 0; i < f.points_num; i++) {
            points[i] = f.points[i];
            azimuth[i] = f.azimuth[i];
            elevation[i] = f.elevation[i];
            distances[i] = f.distances[i];
            reflectivities[i] = f.reflectivities[i];
        }
        for (int i = 0; i < f.packet_num; i++) {
            sensor_timestamp[i] = f.sensor_timestamp[i];
            azimuths[i] = f.azimuths[i];
        }
    }
    uint64_t host_timestamp;
    uint64_t sensor_timestamp[hesai::lidar::kMaxPacketNumPerFrame];
    uint8_t major_version;
    uint8_t minor_version;
    uint16_t return_mode;
    uint16_t spin_speed;
    uint32_t points_num;
    uint32_t packet_num;
    hesai::lidar::LidarPointXYZIRT points[
            hesai::lidar::kMaxPacketNumPerFrame *
            hesai::lidar::kMaxPointsNumPerPacket];
    uint16_t azimuths[hesai::lidar::kMaxPacketNumPerFrame];
    uint8_t reflectivities[
            hesai::lidar::kMaxPacketNumPerFrame *
            hesai::lidar::kMaxPointsNumPerPacket];
    float azimuth[
            hesai::lidar::kMaxPacketNumPerFrame *
            hesai::lidar::kMaxPointsNumPerPacket];
    float elevation[
            hesai::lidar::kMaxPacketNumPerFrame *
            hesai::lidar::kMaxPointsNumPerPacket];
    uint16_t distances[
            hesai::lidar::kMaxPacketNumPerFrame *
            hesai::lidar::kMaxPointsNumPerPacket];
    uint16_t block_num;
    uint16_t laser_num;
    uint16_t packet_index;
    bool scan_complete;
    double distance_unit;
    int frame_index;
    uint8_t lidar_state;
    uint8_t work_mode;
};

#endif
