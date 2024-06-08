# ---------------------------------------------------
# CMakeLists.txt
#
# Author: Thomas Choquet <thomas.publique@icloud.com>
# Date: 2024/05/25 13:03:58
# ---------------------------------------------------

enable_language(C)

add_executable(Window "window.c")
set_target_properties(Window PROPERTIES FOLDER "exemples")
target_link_libraries(Window mlx)

add_executable(put_image_to_window "put_image_to_window.c")
set_target_properties(put_image_to_window PROPERTIES FOLDER "exemples")
target_link_libraries(put_image_to_window mlx)

add_executable(pixel_format "pixel_format.c")
set_target_properties(pixel_format PROPERTIES FOLDER "exemples")
target_link_libraries(pixel_format mlx)