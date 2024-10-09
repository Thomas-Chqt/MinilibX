MinilibX
========

**MinilibX** is a cross-platform implementation of the [42's minilibx](https://github.com/42Paris/minilibx-linux), a very basic graphic library used at 42.

This implementation does not provide all the features of the original library, but only the ones I use in my projects.

Graphics with MLX
-----------------

To draw onto a window using MiniLibX, you first need to create an `mlx_image` with `mlx_new_image`. This image acts as a pixel buffer. You can obtain a pointer to this buffer using `mlx_get_data_addr`. After retrieving the pointer, you can write the desired pixel color into the buffer, where each pixel is represented by a 4-byte integer.

Finally, to display the image on the window, use `mlx_put_image_to_window`. An image can be reused multiple times, making rendering the same content more efficient than redrawing each pixel every frame.

For more detailed usage of MiniLibX, please refer to the [42Docs website](https://harm-smits.github.io/42docs/libs/minilibx).

Examples are also available in the `example` directory.

Projects using MLX
------------------

I use this library in two projects, both created at 42 Tokyo:

- [FdF - A Wireframe Map Renderer](https://github.com/Thomas-Chqt/FdF)
- [cub3d - A first-person shooter game using raycasting](https://github.com/Thomas-Chqt/cub3d)

Build
-----

CMake is used as the build system. The recommended way to use the library is to clone the repository inside your project and use `add_subdirectory`.

Alternatively, the library can be built separately using CMake.

```sh
mkdir build
cmake -S . -B build
cmake --build build
```

| CMake Option          | Default Value | Description                |
|-----------------------|---------------|----------------------------|
| `BUILD_SHARED_LIBS`   | OFF           | Build as shared library    |
| `MLX_BUILD_EXAMPLES`  | OFF           | Build the examples         |