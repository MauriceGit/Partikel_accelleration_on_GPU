# Particle-Simulation using the GPU via the OpenGL Compute-Shader

This particle simulation is more like a proof of concept for myself or others, to show and understand how compute shaders in OpenGL work.
Using the knowledge I gathered from this project, more complex, fascinating and fast projects are possible, which use the power of
the graphics hardware for computations.

The final result looks like that:

![Particle-Simulation](https://github.com/MauriceGit/Partikel_accelleration_on_GPU/blob/master/Screenshots/screenshot_31.png "Particle-Simulation screenshot")

## **Animation**

Each particle has a life span in which it fades from red (new born) to blue (about dying).
When a particle dies, it respawnes in its negated (x,y,z) position.

The movement itself is computed using an Euler integration method for each particle.

There are several global randomly moving attraction points. Particles are attracted and accelerated by these points.

Example screenshots:

![Particle-Simulation](https://github.com/MauriceGit/Partikel_accelleration_on_GPU/blob/master/Screenshots/screenshot_13.png "Particle-Simulation screenshot")
![Particle-Simulation](https://github.com/MauriceGit/Partikel_accelleration_on_GPU/blob/master/Screenshots/screenshot_14.png "Particle-Simulation screenshot")
![Particle-Simulation](https://github.com/MauriceGit/Partikel_accelleration_on_GPU/blob/master/Screenshots/screenshot_27.png "Particle-Simulation screenshot")

## **Install && Run**

I tested an ran this simulation on a debian-based Linux OS (Ubuntu, Mint, ...). It should run on other machines as well but is not
tested.

### **System Requirements**

The following system attributes are required for running this simulation:

- A graphics card supporting OpenGL version 4.3 (For the compute-shader!).

- Unix-Libraries: xorg-dev and mesa-common-dev

Both GLEW (The OpenGL Extension Wrangler Library) and GLFW (Window-Manager) are now included locally, so no system wide
installation is necessary!

For a system wide installation, further information is available under the following links:

- GLFW: [http://www.glfw.org/](http://www.glfw.org/).

- GLEW (with install instructions): [http://glew.sourceforge.net/build.html](http://glew.sourceforge.net/build.html).

### **Running**

Compiling and running should work under windows. OSX is not supported as the available OpenGL version does not reach 4.3.
In the root directory, execute:

- cmake .

- make

- cd bin/

- ./particleSim

While the simulation runs, you can move around (always looking to the center!) with your mouse (left-klick and move) and zoom in/out with your mouse (right click and move up/down).

## **More Screenshots**

So, here are some more screenshots, because screenshots are awesome :)

![Particle-Simulation](https://github.com/MauriceGit/Partikel_accelleration_on_GPU/blob/master/Screenshots/screenshot_35.png "Particle-Simulation screenshot")
![Particle-Simulation](https://github.com/MauriceGit/Partikel_accelleration_on_GPU/blob/master/Screenshots/screenshot_39.png "Particle-Simulation screenshot")
![Particle-Simulation](https://github.com/MauriceGit/Partikel_accelleration_on_GPU/blob/master/Screenshots/screenshot_40.png "Particle-Simulation screenshot")
![Particle-Simulation](https://github.com/MauriceGit/Partikel_accelleration_on_GPU/blob/master/Screenshots/screenshot_30.png "Particle-Simulation screenshot")
![Particle-Simulation](https://github.com/MauriceGit/Partikel_accelleration_on_GPU/blob/master/Screenshots/screenshot_0.png "Particle-Simulation screenshot")
![Particle-Simulation](https://github.com/MauriceGit/Partikel_accelleration_on_GPU/blob/master/Screenshots/screenshot_8.png "Particle-Simulation screenshot")
![Particle-Simulation](https://github.com/MauriceGit/Partikel_accelleration_on_GPU/blob/master/Screenshots/screenshot_10.png "Particle-Simulation screenshot")
![Particle-Simulation](https://github.com/MauriceGit/Partikel_accelleration_on_GPU/blob/master/Screenshots/screenshot_11.png "Particle-Simulation screenshot")
![Particle-Simulation](https://github.com/MauriceGit/Partikel_accelleration_on_GPU/blob/master/Screenshots/screenshot_22.png "Particle-Simulation screenshot")

You can find some more screenshots in the screenshot-folder: [More Screenshots :)](https://github.com/MauriceGit/Partikel_accelleration_on_GPU/tree/master/Screenshots).




