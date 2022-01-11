#version 330 core

// Positions
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec3 aColor;
// Texture coordinates
layout (location = 2) in vec2 aTex;

// Outputs the current position for the fragment shader
out vec3 currentPos;
// Outputs the color for the fragment shader
out vec3 color;
// Outputs the texture coordinates to the fragment shader
out vec2 texCoord;

// Model view projection matrices 
uniform mat4 camMatrix;
uniform mat4 model;

void main()
{
	currentPos = vec3(model * vec4(aPos, 1.0f));

	color = aColor;

	texCoord = aTex;

	gl_Position = camMatrix * vec4(currentPos, 1.0);
}
