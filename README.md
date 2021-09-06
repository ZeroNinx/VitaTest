# VitaTest

PSV自制软件开发

用于学习图形学（OpenGL）

可能会发展成某个游戏或引擎~~或者更可能烂尾~~

> ~~总之不要抱太高期望....~~

## Building
此项目依赖 PVR_PSP2。 所需的模块（libgpu_es4_ext.suprx、libIMGEGL.suprx、libGLESv2.suprx 和 libpvrPSP2_WSEGL.suprx）需要放在module文件夹内。
同时编译需要用到3个库：libgpu_es4_ext_stub_weak、libGLESv2_stub_weak 和 libIMGEGL_stub_weak。然后使用以下命令构建：

```
mkdir build && cd build
cmake .. && make
```

## Credit

- PVR_PSP2 https://github.com/GrapheneCt/PVR_PSP2
- VitaSDK https://github.com/vitasdk