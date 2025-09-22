//src/ui/events.rs
#[cxx::bridge]
mod ffi {
    extern "Rust" {
        fn test_event(input: u32) -> u32;
    }
}


fn test_event(input: u32) -> u32 {
    input * 2
}