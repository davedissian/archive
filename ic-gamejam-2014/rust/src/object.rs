extern crate rsfml;

use Vec2 = rsfml::system::Vector2f;
use rsfml::graphics::{RectangleShape, Color};

use context;

pub struct Object<'p>
{
  position: Vec2,

  // Renderable
  shape: RectangleShape<'p>,
}

impl<'p> Object<'p>
{
  // Constructor
  pub fn new(p: Vec2) -> Object
  {
    // Set up the object
    let mut object = Object
    {
      position: p,
      shape: match RectangleShape::new()
      {
        Some(shape) => shape,
        None => fail!("ERROR: Cannot create the shape")
      }
    };

    object.shape.set_size(&Vec2::new(50.0, 50.0));
    object.shape.set_fill_color(&Color::white());
    object.shape.set_position(&object.position);

    // Return the object
    object
  }

  // Update
  pub fn update(&mut self)
  {
  }

  // Draw
  pub fn draw(&mut self, cxt: &mut context::Context)
  {
    let local_position = self.position - cxt.camera_position;
    self.shape.set_position(&local_position);
    cxt.window.draw(&self.shape);
  }
}
