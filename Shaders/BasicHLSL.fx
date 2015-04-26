
//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
texture gCellTexture;      // Color texture for mesh
float4x4 gWorldViewProjectionMatrix;      // World * View * Projection matrix
float gParticleFadeFactor;
float gParticleTime;
float gParticleSpeed;


//--------------------------------------------------------------------------------------
// Texture samplers
//--------------------------------------------------------------------------------------
sampler CellTextureSampler = 
sampler_state
{
    Texture = <gCellTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};


//--------------------------------------------------------------------------------------
// Vertex shader output structure
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position   : POSITION;   // vertex position 
    float4 Diffuse    : COLOR0;     // vertex diffuse color (note that COLOR0 is clamped from 0..1)
    float2 TextureUV  : TEXCOORD0;  // vertex texture coords 
};


//--------------------------------------------------------------------------------------
// Pixel shader output structure
//--------------------------------------------------------------------------------------
struct PS_OUTPUT
{
    float4 RGBColor : COLOR0;  // Pixel color    
};


//--------------------------------------------------------------------------------------
// This shader computes standard transform
//--------------------------------------------------------------------------------------
VS_OUTPUT RenderSceneVS( float4 vPos : POSITION,
						 float4 vDiffuse : COLOR0,
                         float2 vTexCoord0 : TEXCOORD0)
{
    VS_OUTPUT Output;
      
    Output.Position = mul(vPos, gWorldViewProjectionMatrix);
    Output.Diffuse = vDiffuse;    
	Output.TextureUV = vTexCoord0; 
    
    return Output;    
}


//--------------------------------------------------------------------------------------
// This shader outputs the pixel's color by modulating the texture's
//       color with diffuse material color
//--------------------------------------------------------------------------------------
PS_OUTPUT RenderScenePS( VS_OUTPUT In) 
{ 
    PS_OUTPUT Output;

    // Lookup mesh texture and modulate it with diffuse
    Output.RGBColor = tex2D(CellTextureSampler, In.TextureUV) * In.Diffuse;

    return Output;
}


//--------------------------------------------------------------------------------------
// Renders scene to render target
//--------------------------------------------------------------------------------------
technique RenderSceneWithTexture
{
    pass P0
    {          
        VertexShader = compile vs_3_0 RenderSceneVS();
        PixelShader  = compile ps_3_0 RenderScenePS();

		DestBlend 			= 	INVSRCALPHA;
		SrcBlend 			= 	SRCALPHA;
		AlphaBlendEnable	= 	TRUE;
		CullMode = NONE;
    }
}


//--------------------------------------------------------------------------------------
// This shader computes standard transform
//--------------------------------------------------------------------------------------
VS_OUTPUT RenderParticleVS( float4 vPos : POSITION,
						 float3 vNormal : NORMAL,
						 float4 vDiffuse : COLOR0,
                         float2 vTexCoord0 : TEXCOORD0)
{
    VS_OUTPUT Output;
    
	float4 vertexOffset = float4(vNormal * gParticleSpeed * gParticleTime, 0.0);
    Output.Position = mul((vPos + vertexOffset), gWorldViewProjectionMatrix);
    Output.Diffuse = vDiffuse;    
	Output.TextureUV = vTexCoord0; 
    
    return Output;    
}


//--------------------------------------------------------------------------------------
// This shader outputs the pixel's color by modulating the texture's
//       color with diffuse material color
//--------------------------------------------------------------------------------------
PS_OUTPUT RenderParticlePS( VS_OUTPUT In) 
{ 
    PS_OUTPUT Output;

    // Lookup mesh texture and modulate it with diffuse
    Output.RGBColor = tex2D(CellTextureSampler, In.TextureUV) * In.Diffuse;
	Output.RGBColor.w *= gParticleFadeFactor;
    return Output;
}

//--------------------------------------------------------------------------------------
// Renders particle to render target
//--------------------------------------------------------------------------------------
technique RenderParticle
{
    pass P0
    {          
        VertexShader = compile vs_3_0 RenderParticleVS();
        PixelShader  = compile ps_3_0 RenderParticlePS();

		DestBlend 			= 	INVSRCALPHA;
		SrcBlend 			= 	SRCALPHA;
		AlphaBlendEnable	= 	TRUE;
		CullMode = NONE;
    }
}
