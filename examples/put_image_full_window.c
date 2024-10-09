/*
 * ---------------------------------------------------
 * put_image_full_window.c
 *
 * Author: Thomas Choquet <thomas.publique@icloud.com>
 * Date: 2024/06/13 16:54:08
 * ---------------------------------------------------
 */

#include <stdlib.h>
#include <string.h>

#include "mlx/mlx.h"

typedef struct mlx_data
{
    void* mlx;
    void* window;
    void* img;
} mlx_data;

mlx_data data;

int main()
{
    if ((data.mlx = mlx_init()) == NULL)
        return EXIT_FAILURE;

    if ((data.window = mlx_new_window(data.mlx, 1280, 720, "Window exemple")) == NULL)
        return EXIT_FAILURE;

    if ((data.img = mlx_new_image(data.mlx, 1280, 1280)) == NULL)
        return EXIT_FAILURE;

    int bits_per_pixel, size_line, endian;
    int* bytes = (int*)mlx_get_data_addr(data.img, &bits_per_pixel, &size_line, &endian);

    for (int i = 0; i < 1280 * 720; i++)
        bytes[i] = 0x00ffffff;

    mlx_hook(data.window, 17, 0, (int(*)())exit, (void*)EXIT_SUCCESS);

    mlx_put_image_to_window(data.mlx, data.window, data.img, 0, 0);

    mlx_loop(data.mlx);

    return 0;
}