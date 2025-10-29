#version 330 core
layout (location = 0) in vec4 aVertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

// >>>>>>>>>>>>>>>>>
// Allow for multiple post-processing modes in a single shader
//
// 0: None
// 1 << 0 (1): Chaos
// 1 << 1 (2): Confuse
// 1 << 2 (4): Shake
// >>>>>>>>>>>>>>>>>
uniform int u_Mode;
uniform float u_Time;

void main()
{
	gl_Position = vec4(aVertex.xy, 0.0f, 1.0f);
	vec2 texCoords = aVertex.zw;

	if ((u_Mode & 1) != 0) // Enable "chaos"
	{
		float strength = 0.3f;
		vec2 pos = vec2(texCoords.x + sin(u_Time) * strength, texCoords.y + cos(u_Time) * strength);
		TexCoords = pos;
	}
	else if ((u_Mode & 2) != 0) // Enable "confuse"
	{
		TexCoords = vec2(1.0 - texCoords.x, 1.0 - texCoords.y);
	}
	else 
	{
		TexCoords = texCoords;
	}

	if ((u_Mode & 4) != 0) // Enable "shake"
	{
		float strength = 0.01f;
		gl_Position.x += cos(u_Time * 10) * strength;
		gl_Position.y += cos(u_Time * 15) * strength;
	}
}