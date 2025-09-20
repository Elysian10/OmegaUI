// src/main.rs

#[cxx::bridge]
mod ffi {
    unsafe extern "C++" {
        include!("ui.h");

        type UI;
        fn new_ui() -> UniquePtr<UI>;
        fn should_close(&self) -> bool;
        fn poll_events(self: Pin<&mut UI>);
        fn swap_buffers(self: Pin<&mut UI>);
    }

    extern "Rust" {
        fn my_rust_function(input: u32) -> u32;
    }
}

fn main() {
    let mut ui = ffi::new_ui();
    if ui.is_null() {
        eprintln!("Failed to create UI");
        return;
    }

    println!("UI created successfully!");

    // Main event loop
    while !ui.should_close() {
        // Use as_mut() for each individual call
        ui.as_mut().unwrap().poll_events();

        // Your rendering code would go here

        ui.as_mut().unwrap().swap_buffers();
    }

    println!("Window closed");
}

fn my_rust_function(input: u32) -> u32 {
    input * 2
}
