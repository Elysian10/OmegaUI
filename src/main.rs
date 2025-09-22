#[cxx::bridge]
mod ffi {
    unsafe extern "C++" {
        include!("ui.h");
        type UI;
        fn new_ui() -> UniquePtr<UI>;
        fn run_event_loop(self: Pin<&mut UI>);
    }

    extern "Rust" {
        fn test_event(input: u32) -> u32;
        fn handle_key_event(key: i32, scancode: i32, action: i32, mods: i32, key_name: String);
        fn handle_cursor_event(x: f64, y: f64);
        fn handle_mouse_button_event(button: i32, action: i32, mods: i32);
    }
}

fn test_event(input: u32) -> u32 {
    input * 2
}

fn main() {
    let mut ui = ffi::new_ui();
    if ui.is_null() {
        eprintln!("Failed to create UI");
        return;
    }
    println!("UI created successfully!");
    ui.as_mut().unwrap().run_event_loop();
    println!("Window closed");
}

pub fn handle_key_event(key: i32, scancode: i32, action: i32, mods: i32, key_name: String) {
    println!("Key event: key={}, scancode={}, action={}, mods={}, name='{}'", 
             key, scancode, action, mods, key_name);
    
    match key_name.as_str() {
        "w" => println!("W key pressed - move forward"),
        "a" => println!("A key pressed - move left"),
        "s" => println!("S key pressed - move backward"),
        "d" => println!("D key pressed - move right"),
        " " => println!("Space bar pressed - jump"),
        _ => {} // Ignore other keys or handle by key code
    }
    

    match key {
        256 => println!("Escape key pressed"),
        265 => println!("Up arrow pressed"),
        264 => println!("Down arrow pressed"),
        _ => {}
    }
}

#[allow(non_snake_case)]
pub fn handle_cursor_event(x: f64, y: f64) {
    println!("Cursor position: x={}, y={}", x, y);
}

pub fn handle_mouse_button_event(button: i32, action: i32, mods: i32) {
    let button_name = match button {
        0 => "Left",
        1 => "Right", 
        2 => "Middle",
        3 => "Side 1",
        4 => "Side 2",
        _ => "Unknown"
    };

    println!("Mouse button: {}", button_name);
}