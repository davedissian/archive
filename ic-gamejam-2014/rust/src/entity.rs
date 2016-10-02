extern crate rsfml;

use Vec2 = rsfml::system::Vector2f;
use rsfml::window::keyboard;
use rsfml::graphics::{CircleShape, Color};
use std::num::pow;

use std::rc::Rc;

use context;
use object;

pub struct Entity<'e, 'p>
{
  pub position: Vec2,
  velocity: Vec2,

  // Posessed object
  posessed: Option<&object::Object<'p>>,

  // Renderable
  shape: CircleShape<'e>,
}

impl<'e, 'p> Entity<'e, 'p>
{
  // Constructor
  pub fn new(p: Vec2) -> Entity
  {
    // Set up the object
    let mut object = Entity
    {
      position: p,
      velocity: Vec2::new(0.0, 0.0),
      posessed: None,
      shape: match CircleShape::new()
      {
        Some(shape) => shape,
        None => fail!("ERROR: Cannot create the shape")
      }
    };

    object.shape.set_radius(20.0);
    object.shape.set_fill_color(&Color::blue());
    object.shape.set_position(&object.position);

    // Return the object
    object
  }

  // Update
  pub fn update(&mut self, objects: &'p Vec<object::Object<'p>>)
  {
    let speed = 0.001;
    let attenuation = 0.9975;

    if keyboard::is_key_pressed(keyboard::W) { self.velocity.y -= speed; }
    if keyboard::is_key_pressed(keyboard::S) { self.velocity.y += speed; }
    if keyboard::is_key_pressed(keyboard::A) { self.velocity.x -= speed; }
    if keyboard::is_key_pressed(keyboard::D) { self.velocity.x += speed; }

    self.position.x += self.velocity.x;
    self.position.y += self.velocity.y;

    // Attenuate velocity
    self.velocity.x *= attenuation;
    self.velocity.y *= attenuation;

    // Check for collision between this and other objects
    for o in objects.iter()
    {
      let distance = pow(o.position.x - self.position.x, 2) + pow(o.position.y - self.position.y, 2);
      if distance < (32.0 * 32.0)
      {
        self.posessed = Some(o);
        break;
      }
    }
  }

  // Draw
  pub fn draw(&mut self, cxt: &mut context::Context)
  {
    let local_position = self.position - cxt.camera_position;
    self.shape.set_position(&local_position);
    cxt.window.draw(&self.shape);
  }
}
