// 输入：模型空间坐标 (x,y,z)
// 输出：裁剪空间坐标 (x',y',z',w) + UV
float4 VS(float3 pos : POSITION, float2 uv : TEXCOORD) : SV_POSITION {
    return mul(modelViewProjMatrix, float4(pos, 1.0)); // 坐标变换
}