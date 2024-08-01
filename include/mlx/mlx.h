/*
 * ---------------------------------------------------
 * mlx.h
 *
 * Author: Thomas Choquet <thomas.publique@icloud.com>
 * Date: 2024/05/25 12:32:19
 * ---------------------------------------------------
 */

#ifndef MLX_H
# define MLX_H

#ifdef __cplusplus
namespace mlx
{
extern "C"
{
#endif

void* mlx_init();

void* mlx_new_window(void* mlx_ptr, int width, int height, char* title);
int mlx_destroy_window(void* mlx_ptr, void* win_ptr);

void* mlx_new_image(void* mlx_ptr, int width, int height);
int mlx_destroy_image(void* mlx_ptr, void* img_ptr);

char* mlx_get_data_addr(void* img_ptr, int* bits_per_pixel, int* size_line, int* endian);

int mlx_put_image_to_window(void* mlx_ptr, void* win_ptr, void* img_ptr, int x, int y);

int mlx_hook(void* win_ptr, int x_event, int x_mask, int (*func)(), void* param);
int mlx_loop_hook(void* mlx_ptr, int (*func)(), void* param);
int mlx_loop(void* mlx_ptr);

int mlx_mouse_hide();
int mlx_mouse_show();
int mlx_mouse_move(void *win_ptr, int x, int y);
int mlx_mouse_get_pos(void *win_ptr, int *x, int *y);

void* mlx_xpm_file_to_image(void *mlx_ptr, char *filename, int *width, int *height);
void* mlx_png_file_to_image(void *mlx_ptr, char *file, int *width, int *height);

#ifdef __cplusplus
}
}
#endif
#endif // MLX_H