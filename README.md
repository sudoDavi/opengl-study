# OpenGL Study

## Table of Contents
+ [About](#about)
+ [Usage](#usage)
+ [Contributing](#contributing)

## About <a name = "about"></a>

A simple repo for the collection of programs I did while studying OpenGL.
All the programs up until now have been from [LearnOpenGL](https://learnopengl.com).

### Prerequisites

You'll need a modern C++ compiler that supports at least C++17, also you'll need a copy of the [**GLFW**](https://github.com/glfw/glfw) library,
and **CMake**, everything else should be included.

### Installing

Make a folder for the build and use CMake on that, make sure to link against any libraries it complains about, specially
GLFW, that's the main dependency of the project.

The way you'll use the compiler will vary from compiler to compiler.

Here is a linux example:
```
cd opengl-study/
mkdir build
cd build/
cmake ../
make
```

## Usage <a name = "usage"></a>

And you're done, every compiled program should be inside of its own folder.
Like the `Shaders` program, which after built will be inside `build/shader/` with the name `Shaders`.


## Contributing <a name = "contributing"></a>

1. Fork it (https://github.com/sudoDavi/opengl-study/fork)
1. Create your feature branch (git checkout -b feature)
1. Commit your changes (git commit -am 'Add some feature')
1. Push to the branch (git push origin feature)
1. Create a new Pull Request
