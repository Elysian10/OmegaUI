#[cxx::bridge]
mod ffi {
    unsafe extern "C++" {
        include!("ui.h"); // Just the filename
        
        type UI;
        fn new_ui() -> UniquePtr<UI>;
    }
}

fn main() {
    let _client = ffi::new_ui();
    println!("ui created successfully!");
}
