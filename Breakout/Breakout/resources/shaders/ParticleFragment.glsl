#version 330 core

in vec2 TexCoords;
in vec4 ParticleColor;

out vec4 FragColor;

uniform sampler2D u_Sprite;

void main()
{
	FragColor = texture(u_Sprite, TexCoords) * ParticleColor;
}