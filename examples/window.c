/*
 * ---------------------------------------------------
 * window.c
 *
 * Author: Thomas Choquet <thomas.publique@icloud.com>
 * Date: 2024/05/25 13:04:18
 * ---------------------------------------------------
 */

#include <stdlib.h>

#include "mlx/mlx.h"

int main()
{
    void* mlx = mlx_init();
    if (mlx == NULL)
        return EXIT_FAILURE;

    void* window = mlx_new_window(mlx, 1280, 720, "Window exemple");
    if (window == NULL)
        return EXIT_FAILURE;

    mlx_hook(window, 17, 0, (int(*)())exit, (void*)EXIT_SUCCESS);

    mlx_loop(mlx);

    return 0;
}