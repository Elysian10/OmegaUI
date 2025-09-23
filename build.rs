use std::env;
use std::path::Path;
use std::process::Command;

fn main() {
    // Set up vcpkg if needed
    let vcpkg_root = Path::new("vcpkg");
    let vcpkg_exe = vcpkg_root.join("vcpkg");

    if !vcpkg_exe.exists() {
        println!("cargo:warning=vcpkg not found, setting it up...");

        // Clone vcpkg
        let status = Command::new("git").args(["clone", "https://github.com/Microsoft/vcpkg.git"]).status();

        if status.is_err() || !status.unwrap().success() {
            panic!("Failed to clone vcpkg");
        }

        // Bootstrap vcpkg
        let bootstrap_status = Command::new("vcpkg/bootstrap-vcpkg.sh").arg("-disableMetrics").status();

        if bootstrap_status.is_err() || !bootstrap_status.unwrap().success() {
            panic!("Failed to bootstrap vcpkg");
        }
    }

    // Install dependencies using manifest mode
    let output = Command::new(&vcpkg_exe).arg("install").arg("--feature-flags=manifests").current_dir(env::current_dir().unwrap()).output();

    match output {
        Ok(output) => {
            if !output.status.success() {
                println!("cargo:warning=vcpkg install failed: {}", String::from_utf8_lossy(&output.stderr));
                panic!("vcpkg install failed");
            }
        }
        Err(e) => {
            panic!("Failed to run vcpkg: {}", e);
        }
    }

    let installed_dir = Path::new("vcpkg_installed/x64-linux");
    let include_dir = installed_dir.join("include");
    let lib_dir = installed_dir.join("lib");

    // Build the CXX bridge and C++ code with vcpkg includes
    let mut build = cxx_build::bridges(&["src/main.rs", "src/ui/events.rs"]);
    build.files(&["cxx/src/ui.cc", "cxx/src/glfw_window.cc"]).include("cxx/include").flag_if_supported("-std=c++23");

    build.include(include_dir.clone());

    let out_dir = env::var("OUT_DIR").unwrap();
    let cxxbridge_include = Path::new(&out_dir).join("cxxbridge");
    build.include(cxxbridge_include);
    build.include(include_dir.join("skia"));

    build.compile("omega-cxx");

    println!("cargo:rustc-link-search={}", lib_dir.display());
    println!("cargo:rustc-link-lib=glfw3");
    println!("cargo:rustc-link-lib=skia");

    println!("cargo:rerun-if-changed=cxx/src");
    println!("cargo:rerun-if-changed=cxx/include");
    println!("cargo:rerun-if-changed=vcpkg.json");
}
