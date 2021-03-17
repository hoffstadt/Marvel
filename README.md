## Marvel

[![build status](https://github.com/hoffstadt/Marvel/workflows/Windows%20Builds/badge.svg?branch=master)](https://github.com/hoffstadt/Marvel/actions?workflow=Windows%20Builds)
[![build status](https://github.com/hoffstadt/Marvel/workflows/Linux%20Builds/badge.svg?branch=master)](https://github.com/hoffstadt/Marvel/actions?workflow=Linux%20Builds)

This repo is the working location of the eventual [Dear PyGui](https://github.com/hoffstadt/DearPyGui) 3D Engine. It also contains several single file examples of creating a triangle with various graphics APIs. These were collected and modified from various sources as a starting point.

This project is still early and parts of it are being ported over from earlier prototypes.

### Hello Triangle
* Demo_Directx11 - D3D11 Hello Triangle
* Demo_Directx12 - D3D12 Hello Triangle
* Demo_OpenGL - OpenGL Hello Triangle
* Demo_Vulkan - Vulkan Hello Triangle
* Demo_WebGL - WebGL Hello Triangle

### 3D Engine
* Marvel_vulkan - Vulkan 3D Engine
* Marvel_d3d11 - Directx 11 3D Engine
* Marvel_opengl - OpenGL 3D Engine
* Marvel_webgl - WebGL 3D Engine - [demo](https://hoffstadt.github.io/Marvel/)

### Progress
| Step | WebGL | OpenGL | Directx 11 | Directx 12 | Vulkan | Metal |
|---------|-------|--------|------------|------------|--------|-------|
| Windows Support | - | ✅ | ✅  | ✅ | ✅ |
| Linux Support | - | ✅ | - | - | ✅ | - |
| Apple Support | - | | - | - |
| Swap Chain Created | - | - | ✅ | | ✅ | |
| Pipeline Abstraction | ✅ | ✅ | ✅ |
| Bindable System | ✅ | ✅ | ✅ |
| Vertex Buffer Layout System | ✅ | ✅ | ✅ |
| Textures | ✅ | ✅ | ✅ |
| Constant Buffer System | ✅ | ✅ | ✅ |
| Camera System | ✅ | ✅ |
| Dynamic Lighting | ✅ | | ✅ |
| Environment Map |
| Alpha Blending |
| Normal Mapping |
| Material System |
| Scene Graph System | | | ✅ |
| Model Loading | | | ✅ |
| Outline Effect |
| Shadows|

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
git clone --recursive https://github.com/hoffstadt/Marvel
```
4. In Visual Studio, use _Open a local folder_ to open _Marvel_ folder. This will use CMakeLists.txt as the project file.
5. Run _Build All_.

## Gallery
![](https://github.com/hoffstadt/DearPyGui/blob/assets/3d_demo_1.png)
![](https://github.com/hoffstadt/DearPyGui/blob/assets/3d_demo_2.png)
