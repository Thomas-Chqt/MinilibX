/*
 * ---------------------------------------------------
 * imageDraw_frag.glsl
 *
 * Author: Thomas Choquet <thomas.publique@icloud.com>
 * Date: 2024/05/26 19:00:34
 * ---------------------------------------------------
 */

#version 410 core

uniform sampler2D u_texture;

in vec2 texCoord;

out vec4 fragmentColor;
        
void main()
{
    fragmentColor = texture(u_texture, vec2(texCoord.x, 1 - texCoord.y));
}
