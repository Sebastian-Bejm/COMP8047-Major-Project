#version 330 core

// Positions
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec3 aColor;
// Texture coordinates
//layout (location = 2) in vec2 aTex;


// Outputs the current position for the fragment shader
//out vec3 currentPos;
// Outputs the color for the fragment shader
out vec3 color;
// Outputs the texture coordinates to the fragment shader
out vec2 texCoord;

// Model view projection matrices 
//uniform mat4 camMatrix; // proj * view
//uniform mat4 model;

void main()
{
   	// Outputs the positions/coordinates of all vertices
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	// Assigns the colors from the Vertex Data to "color"
	color = aColor;
}
