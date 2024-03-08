use autocxx::prelude::*;
use std::thread;

include_cpp! {
    #include "driver_param.h"
    #include "w_lidar_types.h"
    #include "w_lidar.h"
    #include "lidar_types.h"
    safety!(unsafe)
    generate!("createDriverParam")
    generate!("hesai::lidar::DriverParam")
    generate!("hesai::lidar::UdpPacket")
    generate!("hesai::lidar::kMinPointsOfOneFrame")
    generate!("hesai::lidar::kMaxPacketNumPerFrame")
    generate!("WLidar")
    generate_pod!("WLidarDecodedFrame")
    generate_pod!("WLidarDecodedPacket")
}

unsafe impl Send for ffi::WLidar {}
unsafe impl Sync for ffi::WLidar {}
unsafe impl Send for ffi::hesai::lidar::DriverParam {}
unsafe impl Sync for ffi::hesai::lidar::DriverParam {}
unsafe impl Send for ffi::WLidarDecodedFrame {}
unsafe impl Sync for ffi::WLidarDecodedFrame {}

pub type HesaiFrame = cxx::UniquePtr<ffi::WLidarDecodedFrame>;
pub type FrameCallback = Box<dyn FnMut(HesaiFrame)>;

pub enum HesaiLidarType {
    AT128,
    Pandar40M,
    Pandar40P,
    Pandar64,
    Pandar128,
    PandarXT32,
    PandarXT16,
    QT128,
}

pub fn create_driver_param(
    pcap_path: &str,
    lidar_type: HesaiLidarType,
) -> cxx::UniquePtr<ffi::hesai::lidar::DriverParam> {
    let (angle_path, firetime_path) = match lidar_type {
        HesaiLidarType::AT128 => (
            "correction/angle_correction/AT128E2X_Angle Correction File.dat",
            "correction/firetime_correction/AT128E2X_Firetime Correction File.csv",
        ),
        HesaiLidarType::Pandar40M => (
            "correction/angle_correction/Pandar40M_Angle Correction File.csv",
            "correction/firetime_correction/Pandar40E3X_Firetime Correction File.csv",
        ),
        HesaiLidarType::Pandar40P => (
            "correction/angle_correction/Pandar40P_Angle Correction File.csv",
            "correction/firetime_correction/Pandar40P_Firetime Correction File.csv",
        ),
        HesaiLidarType::Pandar64 => (
            "correction/angle_correction/Pandar64_Angle Correction File.csv",
            "correction/firetime_correction/Pandar64_Firetime Correction File.csv",
        ),
        HesaiLidarType::Pandar128 => (
            "correction/angle_correction/Pandar128E3X_Angle Correction File.csv",
            "correction/firetime_correction/Pandar128E3X_Firetime Correction File.csv",
        ),
        HesaiLidarType::PandarXT32 => (
            "correction/angle_correction/PandarXT_Angle Correction File.csv",
            "correction/firetime_correction/PandarXT_Firetime Correction File.csv",
        ),
        HesaiLidarType::PandarXT16 => (
            "correction/angle_correction/PandarXT-16_Angle Correction File.csv",
            "correction/firetime_correction/PandarXT-16_Firetime Correction File.csv",
        ),
        HesaiLidarType::QT128 => (
            "correction/angle_correction/QT128C2X_Channel_Cofig",
            "correction/firetime_correction/QT128C2X_Firetime Correction File.csv",
        ),
    };

    ffi::createDriverParam(pcap_path, angle_path, firetime_path)
}

pub struct HesaiLidarSdk {
    is_thread_running: bool,
    frame_callback: Option<FrameCallback>,
}

impl HesaiLidarSdk {
    pub fn new() -> Self {
        println!("Hesai Lidar SDK");
        Self {
            is_thread_running: false,
            frame_callback: None,
        }
    }

    pub fn set_frame_callback(&mut self, callback: FrameCallback) {
        self.frame_callback = Some(callback);
    }

    pub fn start(&mut self, driver_param: cxx::UniquePtr<ffi::hesai::lidar::DriverParam>) {
        let (tx, rx) = std::sync::mpsc::channel();
        let handle = thread::spawn(move || {
            let mut lidar = ffi::WLidar::new().within_unique_ptr();
            lidar.pin_mut().Init(driver_param);
            let mut packet_index: u16 = 0;
            println!("Hesai Lidar SDK start");
            let mut finished: bool = false;
            while !finished {
                let frame: UniquePtr<ffi::WLidarDecodedFrame> = lidar.pin_mut().Consume(
                    std::pin::Pin::new(&mut packet_index),
                    std::pin::Pin::new(&mut finished),
                );
                // println!("packet_index: {:?}, finished: {:?}", packet_index, finished);

                if !frame.is_null() {
                    tx.send(frame).unwrap();
                }
            }
            lidar.pin_mut().Finish();
        });

        while !handle.is_finished() {
            let frame = match rx.recv() {
                Ok(frame) => frame,
                Err(_) => break,
            };
            println!("frame: {:?}", frame.as_ref().unwrap().points_num);
            if let Some(ref mut callback) = self.frame_callback {
                callback(frame.into());
            }
        }

        handle.join().unwrap();
    }

    pub fn stop(&mut self) {
        self.is_thread_running = false;
    }
}
