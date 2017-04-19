![](./doc/gltf2-loader.png)


---

# glTF2-loader

`glTF2-loader` is a C++14 [glTF][gltf] ([2.0 only][gltf-2-spec]) parsing library. 

## Building

Bulding should be as simple as executing the following commands:
```
mkdir build
cd build
cmake ..
make
```

If you're building for Android, you should use cmake with `-DANDROID=TRUE`

The build process should output a static library, `libgltf2-loader.a`.

## Documentation

Coming soon. For now, the code should be well-enough documented inline, but we should definitely have some Doxygen thing going on in the near future.

## Tests

Tests, using the googletest framework, can be enabled with the `BUILD_TESTS` CMake variable. An executable will be created in the `test/` subdirectory, in your build directory.

CI coming soon.

## Projects using this library

* [Lugdunum](https://lugdunum3d.github.io)

Feel free to open a pull-request to add your project to the list!

## License

glTF2-loader is licensed under [MIT license](./LICENSE).

glTF2-loader uses the following third party libraries:

* [json.hpp](https://github.com/nlohmann/json): Copyright © 2013-2017 Niels Lohmann

*glTF and the glTF logo are trademarks of the Khronos Group Inc.*

[gltf]: https://github.com/KhronosGroup/glTF
[gltf-2-spec]: https://github.com/KhronosGroup/glTF/tree/2.0/specification/2.0
