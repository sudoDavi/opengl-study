# OpenGL Study

## Table of Contents
+ [About](#about)
+ [Usage](#usage)
+ [Contributing](#contributing)

## About <a name = "about"></a>

A simple repo for the collection of programs I did while studying OpenGL.
All the programs up until now have been from [LearnOpenGL](https://learnopengl.com).

### Prerequisites

You'll need a modern C++ compiler that supports at least C++17 and **CMake**, everything else should be included.

### Installing

Clone the repository with `git clone --recursive` .

Make a folder for the build and use CMake on that.

The way you'll use the compiler will vary from system to system.

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
Like the `Shaders` program, which after built will be inside `build/src/shader/` with the name `Shaders`.


## Contributing <a name = "contributing"></a>

1. Fork it (https://github.com/sudoDavi/opengl-study/fork)
1. Create your feature branch (git checkout -b feature)
1. Commit your changes (git commit -am 'Add some feature')
1. Push to the branch (git push origin feature)
1. Create a new Pull Request
