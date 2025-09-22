//srcui/ui.rs
use cxx::UniquePtr;

use crate::ui::ui::ffi::UI;


#[cxx::bridge]
mod ffi {
    unsafe extern "C++" {
        include!("ui.h");

        type UI;
        fn new_ui() -> UniquePtr<UI>;
        fn should_close(&self) -> bool;
    }
}

pub struct RustUI{
    pub ptr: UniquePtr<UI>
}

impl RustUI{
    pub fn new()->Self{
        let ui =  Self{
            ptr: ffi::new_ui()
        };
        ui
    }
}