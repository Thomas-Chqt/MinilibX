/*
 * ---------------------------------------------------
 * imageDraw.metal
 *
 * Author: Thomas Choquet <thomas.publique@icloud.com>
 * Date: 2024/05/25 18:06:36
 * ---------------------------------------------------
 */

#include <metal_stdlib>
#include "Vertex.hpp"

struct VertexOut
{
    float4 pos [[position]];
    float2 uv;
};

vertex VertexOut imageDraw_vs(uint vID                             [[vertex_id]],
                              constant mlx::Vertex *vertices       [[buffer(0)]],
                              constant metal::float3x3& u_mpMatrix [[buffer(1)]])
{
    return (VertexOut){
        .pos = float4(u_mpMatrix * float3(vertices[vID].pos, 1), 1),
        .uv  = vertices[vID].uv
    };
}

fragment float4 imageDraw_fs(VertexOut in [[stage_in]], metal::texture2d<float> u_texture [[texture(0)]])
{
    constexpr metal::sampler textureSampler(metal::mag_filter::linear, metal::min_filter::linear);

    return u_texture.sample(textureSampler, in.uv);
}