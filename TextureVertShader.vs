#version 330 core

// Positions
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec3 aColor;
// Texture coordinates
layout (location = 2) in vec2 aTex;

// Outputs the current position for the fragment shader
//out vec3 currentPos;
// Outputs the color for the fragment shader
out vec3 color;
// Outputs the texture coordinates to the fragment shader
out vec2 texCoord;

// Model view projection matrices 
//uniform mat4 camMatrix;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	//gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	gl_Position = proj * view * model * vec4(aPos, 1.0);

	color = aColor;

	texCoord = aTex;
}
