use std::env;
use std::path::Path;
use std::process::Command;

fn main() {
    // Build the CXX bridge and C++ code first
    cxx_build::bridge("src/main.rs")
        .file("cxx/src/ui.cc")
        .include("cxx/include")
        .flag_if_supported("-std=c++14")
        .compile("omega-cxx");

    // Set up vcpkg if needed
    let vcpkg_root = Path::new("vcpkg");
    let vcpkg_exe = vcpkg_root.join("vcpkg");
    
    if !vcpkg_exe.exists() {
        println!("cargo:warning=vcpkg not found, setting it up...");
        
        // Clone vcpkg
        let status = Command::new("git")
            .args(["clone", "https://github.com/Microsoft/vcpkg.git"])
            .status();
            
        if status.is_err() || !status.unwrap().success() {
            println!("cargo:warning=Failed to clone vcpkg. Make sure git is installed.");
            return;
        }
        
        // Bootstrap vcpkg
        let bootstrap_status = Command::new("vcpkg/bootstrap-vcpkg.sh")
            .arg("-disableMetrics")
            .status();
            
        if bootstrap_status.is_err() || !bootstrap_status.unwrap().success() {
            println!("cargo:warning=Failed to bootstrap vcpkg.");
            return;
        }
    }
    
    // Install dependencies using manifest mode
    let status = Command::new(&vcpkg_exe)
        .arg("install")
        .arg("--feature-flags=manifests")
        .current_dir(env::current_dir().unwrap())
        .status();
    
    if let Ok(status) = status {
        if status.success() {
            // Use the installed dependencies
            let installed_dir = Path::new("vcpkg_installed/x64-linux");
            let lib_dir = installed_dir.join("lib");
            
            if lib_dir.exists() {
                println!("cargo:rustc-link-search={}", lib_dir.display());
                println!("cargo:rustc-link-lib=glfw3");
            }
        }
    }
    
    println!("cargo:rerun-if-changed=cxx/src/ui.cc");
    println!("cargo:rerun-if-changed=cxx/include/ui.h");
    println!("cargo:rerun-if-changed=vcpkg.json");
}