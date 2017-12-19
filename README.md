OpenGL in a Win32 app
===================
A project which render a cube and and OBJ object using OpenGL under a Win32 window, using the [WinAPI](https://en.wikipedia.org/wiki/Windows_API). The project handles several classes to keep some kind of "order. From vec4 datatype (called `Vector4D` actually), until `TriangleMesh` which is a collection of `Triangle` each one composed by 3 integer indexes and a `Vector4D` normal vector.

Code starts on `OGLWin32.cpp` file with:

```cpp
int APIENTRY wWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
```

Render shows a cube (formed by 6 planes), and a OBJ object rotating. The OBJ models can be downloaded from [here](http://ccg.ciens.ucv.ve/~esmitt/files/models/). The *brigde* between OpenGL and WinAPI is handle by the `OGLApplication` class.

It was tested on Visual Studio 2017, under Windows 10

###### If you want to contribute? Great!