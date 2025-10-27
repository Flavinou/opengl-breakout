
#version 330 core
in  vec2  TexCoords;
out vec4  FragColor;
  
uniform sampler2D u_RenderTarget;
uniform vec2      u_TextureOffsets[9];
uniform int       u_EdgeKernel[9];
uniform float     u_BlurKernel[9];

uniform bool u_Chaos;
uniform bool u_Confuse;
uniform bool u_Shake;

void main()
{
    FragColor = vec4(0.0f);
    vec3 sample[9];

    // sample from texture offsets if using convolution matrix
    if(u_Chaos || u_Shake)
        for(int i = 0; i < 9; i++)
            sample[i] = vec3(texture(u_RenderTarget, TexCoords.st + u_TextureOffsets[i]));

    // process effects
    if (u_Chaos)
    {           
        for(int i = 0; i < 9; i++)
            FragColor += vec4(sample[i] * u_EdgeKernel[i], 0.0f);
        FragColor.a = 1.0f;
    }
    else if (u_Confuse)
    {
        FragColor = vec4(1.0 - texture(u_RenderTarget, TexCoords).rgb, 1.0);
    }
    else if (u_Shake)
    {
        for(int i = 0; i < 9; i++)
            FragColor += vec4(sample[i] * u_BlurKernel[i], 0.0f);
        FragColor.a = 1.0f;
    }
    else
    {
        FragColor =  texture(u_RenderTarget, TexCoords);
    }
}