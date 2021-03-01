## Marvel 3D
This repo is the working location of the eventual [Dear PyGui](https://github.com/hoffstadt/DearPyGui) 3D Engine. It also contains several single file examples of creating a triangle with various graphics APIs. These were collected and modified from various sources as a starting point.

This project is still early and parts of it are being ported over from earlier prototypes.

### Hello Triangle
* Directx11_ht - D3D11 Hello Triangle
* OpenGL_ht - OpenGL Hello Triangle
* Vulkan_ht - Vulkan Hello Triangle

### 3D Engine
* Marvel_d3d11 - Directx 11 3D Engine

## Building
### Windows
**Requirements**
- [_git_](https://git-scm.com/)
- [_Visual Studio 2019_ (windows)](https://visualstudio.microsoft.com/vs/) with the following workflows:
  * Python development
  * Desktop development with C++
  * Game development with C++
- [Vulkan SDK](https://vulkan.lunarg.com/)

Instructions:
1. From within a local directory, enter the following bash commands:
```
git clone --recursive https://github.com/hoffstadt/GraphicsAPIs
```
4. In Visual Studio, use _Open a local folder_ to open _GraphicsAPIs_ folder. This will use CMakeLists.txt as the project file.
5. Run _Build All_.

## Gallery
![](https://github.com/hoffstadt/DearPyGui/blob/assets/3d_demo_1.png)
![](https://github.com/hoffstadt/DearPyGui/blob/assets/3d_demo_2.png)