#version 330 core
layout (location = 0) in vec4 aVertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

uniform bool  u_Chaos;
uniform bool  u_Confuse;
uniform bool  u_Shake;
uniform float u_Time;

void main()
{
    gl_Position = vec4(aVertex.xy, 0.0f, 1.0f); 
    vec2 texture = aVertex.zw;
    if (u_Chaos)
    {
        float strength = 0.3;
        vec2 pos = vec2(texture.x + sin(u_Time) * strength, texture.y + cos(u_Time) * strength);        
        TexCoords = pos;
    }
    else if (u_Confuse)
    {
        TexCoords = vec2(1.0 - texture.x, 1.0 - texture.y);
    }
    else
    {
        TexCoords = texture;
    }
    if (u_Shake)
    {
        float strength = 0.01;
        gl_Position.x += cos(u_Time * 10) * strength;        
        gl_Position.y += cos(u_Time * 15) * strength;        
    }
}  