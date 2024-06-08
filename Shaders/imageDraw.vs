/*
 * ---------------------------------------------------
 * imageDraw_vrtx.glsl
 *
 * Author: Thomas Choquet <thomas.publique@icloud.com>
 * Date: 2024/05/26 19:00:17
 * ---------------------------------------------------
 */

#version 410 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uv;

uniform mat3 u_mpMatrix;

out vec2 texCoord;

void main()
{
    gl_Position = vec4(u_mpMatrix * vec3(pos, 1), 1);
    texCoord    = uv;
}