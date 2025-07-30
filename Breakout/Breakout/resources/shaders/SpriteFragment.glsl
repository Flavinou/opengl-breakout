#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D u_Texture;
uniform vec4 u_Color;

void main()
{
	// Sample the texture at the given texture coordinates
	vec4 texColor = texture(u_Texture, TexCoords);
	
	// Combine the texture color with the uniform color
	FragColor = texColor * u_Color;
}