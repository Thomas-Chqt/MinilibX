/*
 * ---------------------------------------------------
 * pixel_format.c
 *
 * Author: Thomas Choquet <thomas.publique@icloud.com>
 * Date: 2024/05/26 14:03:41
 * ---------------------------------------------------
 */

#include "mlx/mlx.h"

#include <stdlib.h>
#include <string.h>

#define WIDTH 1280
#define HEIGHT 720

typedef struct mlx_data
{
    void* mlx;
    void* window;
    void* img1;
    void* img2;
    void* img3;

    void* img4;
    void* img5;
    void* img6;

    void* img7;
    void* img8;
    void* img9;
} mlx_data;

mlx_data data;

int main()
{
    if ((data.mlx = mlx_init()) == NULL)
        return EXIT_FAILURE;

    if ((data.window = mlx_new_window(data.mlx, WIDTH, HEIGHT, "Window exemple")) == NULL)
        return EXIT_FAILURE;

    if ((data.img1 = mlx_new_image(data.mlx, WIDTH / 3, HEIGHT / 3)) == NULL)
        return EXIT_FAILURE;
    if ((data.img2 = mlx_new_image(data.mlx, WIDTH / 3, HEIGHT / 3)) == NULL)
        return EXIT_FAILURE;
    if ((data.img3 = mlx_new_image(data.mlx, WIDTH / 3, HEIGHT / 3)) == NULL)
        return EXIT_FAILURE;

    if ((data.img4 = mlx_new_image(data.mlx, WIDTH / 3, HEIGHT / 3)) == NULL)
        return EXIT_FAILURE;
    if ((data.img5 = mlx_new_image(data.mlx, WIDTH / 3, HEIGHT / 3)) == NULL)
        return EXIT_FAILURE;
    if ((data.img6 = mlx_new_image(data.mlx, WIDTH / 3, HEIGHT / 3)) == NULL)
        return EXIT_FAILURE;

    if ((data.img7 = mlx_new_image(data.mlx, WIDTH / 3, HEIGHT / 3)) == NULL)
        return EXIT_FAILURE;
    if ((data.img8 = mlx_new_image(data.mlx, WIDTH / 3, HEIGHT / 3)) == NULL)
        return EXIT_FAILURE;
    if ((data.img9 = mlx_new_image(data.mlx, WIDTH / 3, HEIGHT / 3)) == NULL)
        return EXIT_FAILURE;

    int bits_per_pixel, size_line, endian;
    int* bytes = (int*)mlx_get_data_addr(data.img1, &bits_per_pixel, &size_line, &endian);
    for (int i = 0; i < (WIDTH / 3) * (HEIGHT / 3); i++)
        bytes[i] = 0x00ff0000;

    bytes = (int*)mlx_get_data_addr(data.img2, &bits_per_pixel, &size_line, &endian);
    for (int i = 0; i < (WIDTH / 3) * (HEIGHT / 3); i++)
        bytes[i] = 0x0000ff00;

    bytes = (int*)mlx_get_data_addr(data.img3, &bits_per_pixel, &size_line, &endian);
    for (int i = 0; i < (WIDTH / 3) * (HEIGHT / 3); i++)
        bytes[i] = 0x000000ff;


    bytes = (int*)mlx_get_data_addr(data.img4, &bits_per_pixel, &size_line, &endian);
    for (int i = 0; i < (WIDTH / 3) * (HEIGHT / 3); i++)
        bytes[i] = 0x40ff0000;

    bytes = (int*)mlx_get_data_addr(data.img5, &bits_per_pixel, &size_line, &endian);
    for (int i = 0; i < (WIDTH / 3) * (HEIGHT / 3); i++)
        bytes[i] = 0x4000ff00;

    bytes = (int*)mlx_get_data_addr(data.img6, &bits_per_pixel, &size_line, &endian);
    for (int i = 0; i < (WIDTH / 3) * (HEIGHT / 3); i++)
        bytes[i] = 0x400000ff;


    bytes = (int*)mlx_get_data_addr(data.img7, &bits_per_pixel, &size_line, &endian);
    for (int i = 0; i < (WIDTH / 3) * (HEIGHT / 3); i++)
        bytes[i] = 0xf0ff0000;

    bytes = (int*)mlx_get_data_addr(data.img8, &bits_per_pixel, &size_line, &endian);
    for (int i = 0; i < (WIDTH / 3) * (HEIGHT / 3); i++)
        bytes[i] = 0xf000ff00;

    bytes = (int*)mlx_get_data_addr(data.img9, &bits_per_pixel, &size_line, &endian);
    for (int i = 0; i < (WIDTH / 3) * (HEIGHT / 3); i++)
        bytes[i] = 0xf00000ff;

    mlx_put_image_to_window(data.mlx, data.window, data.img1, 0,             0);
    mlx_put_image_to_window(data.mlx, data.window, data.img2, WIDTH / 3,     0);
    mlx_put_image_to_window(data.mlx, data.window, data.img3, WIDTH / 3 * 2, 0);

    mlx_put_image_to_window(data.mlx, data.window, data.img4, 0,             HEIGHT / 3);
    mlx_put_image_to_window(data.mlx, data.window, data.img5, WIDTH / 3,     HEIGHT / 3);
    mlx_put_image_to_window(data.mlx, data.window, data.img6, WIDTH / 3 * 2, HEIGHT / 3);

    mlx_put_image_to_window(data.mlx, data.window, data.img7, 0,             HEIGHT / 3 * 2);
    mlx_put_image_to_window(data.mlx, data.window, data.img8, WIDTH / 3,     HEIGHT / 3 * 2);
    mlx_put_image_to_window(data.mlx, data.window, data.img9, WIDTH / 3 * 2, HEIGHT / 3 * 2);

    mlx_put_image_to_window(data.mlx, data.window, data.img7, 0,             HEIGHT / 3 * 2);
    mlx_put_image_to_window(data.mlx, data.window, data.img8, WIDTH / 3,     HEIGHT / 3 * 2);
    mlx_put_image_to_window(data.mlx, data.window, data.img9, WIDTH / 3 * 2, HEIGHT / 3 * 2);

    mlx_hook(data.window, 17, 0, (int(*)())exit, (void*)EXIT_SUCCESS);

    mlx_loop(data.mlx);

    return 0;
}