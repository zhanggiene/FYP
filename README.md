




1. Download Boost 
https://linux.how2shout.com/how-to-install-boost-c-on-ubuntu-20-04-or-22-04/ 
2. Download Cmake 
3. Download Opengl 
```
sudo apt-get install libglu1-mesa-dev mesa-common-dev libglfw3-dev
export OPENGL_opengl_LIBRARY=/usr/lib/x86_64-linux-gnu/libGL.so
export OPENGL_glx_LIBRARY=/usr/lib/x86_64-linux-gnu/libGLX.so
export OPENGL_INCLUDE_DIR=/usr/include/GL

```

Download all the external dependency

```
cd FYP
git clone https://github.com/ocornut/imgui.git

cd ./imgui

git checkout docking

cd ..
git clone https://github.com/ddemidov/amgcl.git


```

```
cmake .
make
./fyp
```