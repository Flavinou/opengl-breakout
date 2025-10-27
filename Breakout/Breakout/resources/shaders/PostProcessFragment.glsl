#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D u_RenderTarget;
uniform vec2 u_TextureOffsets[9];
uniform int u_EdgeKernel[9];
uniform float u_BlurKernel[9];

// >>>>>>>>>>>>>>>>>
// Allow for multiple post-processing modes in a single shader
//
// 0: None
// 1 << 0 (1): Chaos
// 1 << 1 (2): Confuse
// 1 << 2 (4): Shake
// >>>>>>>>>>>>>>>>>
uniform int u_Mode;

void main()
{
	FragColor = vec4(0.0f);
	vec3 sample[9];

	// Sample from texture offsets if using convolution matrix
	if ((u_Mode & 5) != 0) // Chaos OR shake enabled
	{
		for(int i = 0; i < 9; i++)
            sample[i] = vec3(texture(u_RenderTarget, TexCoords.st + u_TextureOffsets[i]));
	}

	// Process effects
	if ((u_Mode & 1) != 0) // Enable "chaos"
	{
		for(int i = 0; i < 9; i++)
            FragColor += vec4(sample[i] * u_EdgeKernel[i], 0.0f);
        FragColor.a = 1.0f;
	}
	else if ((u_Mode & 2) != 0) // Enable "confuse"
	{
		FragColor = vec4(1.0f - texture(u_RenderTarget, TexCoords).rgb, 1.0f);
	}
	else if ((u_Mode & 4) != 0) // Enable "shake"
	{
		for(int i = 0; i < 9; i++)
            FragColor += vec4(sample[i] * u_BlurKernel[i], 0.0f);
        FragColor.a = 1.0f;
	}
	else
	{
		// No effect, sample from the render target no-op
		FragColor = texture(u_RenderTarget, TexCoords);
	}
}