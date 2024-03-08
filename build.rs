use std::env;

use cmake;
use bindgen;
use autocxx_build;
use copy_to_output::copy_to_output;

fn main() {
    let mut builder = autocxx_build::Builder::new(
        "src/lib.rs",
        &[
            "cpp",
            "cpp/wrapper",
            "cpp/HesaiLidar_SDK_2.0/driver", 
            "cpp/HesaiLidar_SDK_2.0/libhesai/Lidar",
            "cpp/HesaiLidar_SDK_2.0/libhesai",
            "cpp/HesaiLidar_SDK_2.0/libhesai/UdpParser",
            "cpp/HesaiLidar_SDK_2.0/libhesai/UdpParser/include",
            "cpp/HesaiLidar_SDK_2.0/libhesai/UdpParser/src",
            "cpp/HesaiLidar_SDK_2.0/libhesai/UdpProtocol",
            "cpp/HesaiLidar_SDK_2.0/libhesai/Source/include",
            "cpp/HesaiLidar_SDK_2.0/libhesai/Container/include",
            "cpp/HesaiLidar_SDK_2.0/libhesai/Container/src",
            "cpp/HesaiLidar_SDK_2.0/libhesai/PtcClient/include",
            "cpp/HesaiLidar_SDK_2.0/libhesai/PtcParser",
            "cpp/HesaiLidar_SDK_2.0/libhesai/PtcParser/include",
            "cpp/HesaiLidar_SDK_2.0/libhesai/PtcParser/src",
            "cpp/HesaiLidar_SDK_2.0/libhesai/Logger/include",
            "cpp/HesaiLidar_SDK_2.0/libhesai/include",
        ]
    )
        .extra_clang_args(&["-std=c++17"])
        .build().expect("Failed to generate bindings");
    builder
        .flag_if_supported("-std=c++17")
        // .file("cpp/hesai_lidar_sdk.cpp")

        .compile("hesai-lidar-sdk-cpp");
    // let bindings = bindgen::Builder::default()
    //     .enable_cxx_namespaces()
    //     .header("cpp/hesai_lidar_sdk.h")
    //     .clang_args(&[
    //         "-x", "c++",
    //         "-std=c++17",
    //         "-stdlib=libstdc++",
    //         "-I", "cpp/HesaiLidar_SDK_2.0/driver", 
    //         "-I", "cpp/HesaiLidar_SDK_2.0/libhesai/Lidar",
    //         "-I", "cpp/HesaiLidar_SDK_2.0/libhesai",
    //         "-I", "cpp/HesaiLidar_SDK_2.0/libhesai/UdpParser",
    //         "-I", "cpp/HesaiLidar_SDK_2.0/libhesai/UdpParser/include",
    //         "-I", "cpp/HesaiLidar_SDK_2.0/libhesai/UdpParser/src",
    //         "-I", "cpp/HesaiLidar_SDK_2.0/libhesai/UdpProtocol",
    //         "-I", "cpp/HesaiLidar_SDK_2.0/libhesai/Source/include",
    //         "-I", "cpp/HesaiLidar_SDK_2.0/libhesai/Container/include",
    //         "-I", "cpp/HesaiLidar_SDK_2.0/libhesai/Container/src",
    //         "-I", "cpp/HesaiLidar_SDK_2.0/libhesai/PtcClient/include",
    //         "-I", "cpp/HesaiLidar_SDK_2.0/libhesai/PtcParser",
    //         "-I", "cpp/HesaiLidar_SDK_2.0/libhesai/PtcParser/include",
    //         "-I", "cpp/HesaiLidar_SDK_2.0/libhesai/PtcParser/src",
    //         "-I", "cpp/HesaiLidar_SDK_2.0/libhesai/Logger/include",
    //         "-I", "cpp/HesaiLidar_SDK_2.0/libhesai/include",
    //     ])
    //     //.allowlist_recursively(true)
    //     //.allowlist_type("HesaiLidarSdk")
    //     //.allowlist_function("hello")
    //     .generate()
    //     .expect("Unable to generate bindings");
    // let out_path = std::path::PathBuf::from(std::env::var("OUT_DIR").unwrap());
    // println!("out_path: {:?}", out_path);
    // bindings
    //     .write_to_file(out_path.join("bindings.rs"))
    //     .expect("Couldn't write bindings!");

    let dst = cmake::build("cpp");
    println!("cargo:rustc-link-search=native={}", dst.display());
    println!("cargo:rustc-link-lib=static=hesai-lidar-sdk");
    println!("cargo:rustc-link-lib=static=source_lib");
    println!("cargo:rustc-link-lib=static=container_lib");
    println!("cargo:rustc-link-lib=static=ptcClient_lib");
    println!("cargo:rustc-link-lib=static=ptcParser_lib");
    println!("cargo:rustc-link-lib=static=log_lib");
    println!("cargo:rustc-link-lib=static=platutils_lib");
    println!("cargo:rustc-link-lib=dylib=stdc++");
    // add boost(system filesystem thread iostreams) and openssl
    println!("cargo:rustc-link-lib=dylib=boost_system");
    println!("cargo:rustc-link-lib=dylib=boost_thread");
    println!("cargo:rustc-link-lib=dylib=boost_filesystem");
    println!("cargo:rustc-link-lib=dylib=boost_iostreams");
    println!("cargo:rustc-link-lib=dylib=ssl");
    println!("cargo:rustc-link-lib=dylib=crypto");

    // println!("cargo:rerun-if-changed=src/lib.rs");

    copy_to_output("cpp/HesaiLidar_SDK_2.0/correction", &env::var("PROFILE").unwrap()).expect("Failed to copy correction files");
}
