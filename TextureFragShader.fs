#version 330 core

out vec4 FragColor;

// Inputs the current position from the vertex shader
//in vec3 currentPos;

// Inputs the color from the vertex shader
in vec3 color;

in vec2 texCoord;

uniform sampler2D tex0;

void main() 
{
   FragColor = texture(tex0, texCoord);
}
