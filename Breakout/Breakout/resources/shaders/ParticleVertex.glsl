#version 330 core
layout (location = 0) in vec4 aVertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;
out vec4 ParticleColor;

uniform mat4 u_Projection;
uniform vec2 u_Offset;
uniform vec4 u_Color;
uniform float u_Scale;

void main()
{
	TexCoords = aVertex.zw;
	ParticleColor = u_Color;
	gl_Position = u_Projection * vec4((aVertex.xy * u_Scale) + u_Offset, 0.0, 1.0);
}