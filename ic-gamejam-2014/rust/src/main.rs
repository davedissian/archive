extern crate native;
extern crate rsfml;

use std::rand;
use std::rand::distributions::{IndependentSample, Range};

use Vec2 = rsfml::system::Vector2f;
use rsfml::window::event;
use rsfml::graphics::Color;

mod context;
mod window;
mod object;
mod entity;

// This is needed on OS X I believe...
#[start]
fn start(argc: int, argv: **u8) -> int
{
  native::start(argc, argv, main)
}

fn lerp(a: f32, b: f32, t: f32) -> f32 { a + (b - a) * t }

fn main () -> ()
{
  let width = 800;
  let height = 600;
  let mut cxt = context::Context
  {
    window: window::setup(width, height),
    camera_position: Vec2::new(0.0, 0.0)
  };

  // Add the entity
  let mut ent = entity::Entity::new(Vec2::new(100.0, 100.0));

  // Add some objects
  let mut objects = Vec::new();
  let pos = Range::new(0.0, 800.0);
  for _ in range(0, 10)
  {
    let (x, y) = (pos.ind_sample(&mut rand::task_rng()), pos.ind_sample(&mut rand::task_rng()));
    objects.push(object::Object::new(Vec2::new(x as f32, y as f32)));
  }

  // Main Loop
  while cxt.window.is_open()
  {
    // Handle events
    for event in cxt.window.events()
    {
      match event
      {
        event::Closed => cxt.window.close(),
        _ => {}
      }
    }

    // Update objects
    ent.update(&objects);
    for p in objects.mut_iter() { p.update(); }

    // Lerp between entity position and camera position
    let t = 0.0025;
    let next_position = Vec2::new(
      ent.position.x - width as f32 * 0.5,
      ent.position.y - height as f32 * 0.5);
    cxt.camera_position.x = lerp(cxt.camera_position.x, next_position.x, t);
    cxt.camera_position.y = lerp(cxt.camera_position.y, next_position.y, t);

    // Draw
    cxt.window.clear(&Color::new_RGB(0, 200, 200));
    ent.draw(&mut cxt);
    for p in objects.mut_iter() { p.draw(&mut cxt); }
    cxt.window.display()
  }
}