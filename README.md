## Hello Triangle
Single file examples of creating a triangle with various graphics APIs. This were collected and modified from various sources as a starting point.

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
