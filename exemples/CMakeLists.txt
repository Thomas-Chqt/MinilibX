# ---------------------------------------------------
# CMakeLists.txt
#
# Author: Thomas Choquet <thomas.publique@icloud.com>
# Date: 2024/05/25 13:03:58
# ---------------------------------------------------

add_executable(Window "window.c")
target_link_libraries(Window mlx)

add_executable(put_image_to_window "put_image_to_window.c")
target_link_libraries(put_image_to_window mlx)