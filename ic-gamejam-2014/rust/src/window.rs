extern crate rsfml;

use rsfml::window::{ContextSettings, VideoMode, Close};
use rsfml::graphics::RenderWindow;

pub fn setup(width: uint, height: uint) -> RenderWindow
{
  // Create the window of the application
  match RenderWindow::new(VideoMode::new_init(width, height, 32),
    "IC GameJam 2014", Close, &ContextSettings::default())
  {
    Some(window) => return window,
    None => fail!("Cannot create a new Render Window")
  };
}