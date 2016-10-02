Posessed
=========
This is my entry to Imperial College's first Game Jam in June 2014. I initially wrote it in Rust but after running into a stumbling block on the first day I decided to port it to C++ and continue working on it from there. The original Rust code can be found in the Rust directory.

Building
-----
Possessed was developed on a Linux machine so for the time being Linux is the only supported platform. Using your favourite package manager, install SFML 2 and Box2D (for Ubuntu: Install libbox2d-dev and build SFML from source). Then navigate to the root directory of the project, create a build directory, run cmake and build the game using the following commands:

    cd /path/to/game/
    mkdir build
    cd build
    cmake ..
    make
  
Afterwards you'll find the resulting binary in the bin directory.

Gameplay
-----
You control an entity, a drifting soul that can enter inanimate objects and apply a force to them. The aim of the game is to use this ability to place the glowing object in the green target area. This can range from having to stack objects to bursting through walls.

Controls are:

* `WASD` for movement
* `Space` to take control of an object
* `Return` to release an object
