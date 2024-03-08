use hesai_sdk_rs;

fn main() {
    let args: Vec<String> = std::env::args().collect();
    if args.len() < 2 {
        println!("Usage: {} <pcap_file>", args[0]);
        return;
    }

    let absolute_path = std::fs::canonicalize(&args[1]).unwrap();

    let driver_param = hesai_sdk_rs::create_driver_param(
        absolute_path.to_str().unwrap(),
        hesai_sdk_rs::HesaiLidarType::AT128,
    );

    let mut hesai_lidar_sdk = hesai_sdk_rs::HesaiLidarSdk::new();

    let callback = Box::new(|frame: hesai_sdk_rs::HesaiFrame| {
        println!("frame: {:?}", frame.as_ref().unwrap().points_num);
    });

    hesai_lidar_sdk.set_frame_callback(callback);

    hesai_lidar_sdk.start(driver_param);
}
