#version 330 core
layout (location = 0) in vec4 aVertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

uniform mat4 u_Model;
uniform mat4 u_Projection;

void main()
{
	TexCoords = aVertex.zw; // Extracting texture coordinates from the vertex data
	gl_Position = u_Projection * u_Model * vec4(aVertex.xy, 0.0, 1.0); // Transforming vertex position
}