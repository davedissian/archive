extern crate rsfml;

use Vec2 = rsfml::system::Vector2f;
use rsfml::graphics::RenderWindow;

pub struct Context<'c>
{
  pub window: RenderWindow,
  pub camera_position: Vec2,
}