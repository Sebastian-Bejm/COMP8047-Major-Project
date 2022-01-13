#version 330 core

// Positions
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec3 aColor;

// Outputs the current position for the fragment shader
out vec3 currentPos;
// Outputs the color for the fragment shader
out vec3 color;

// Model view projection matrices 
uniform mat4 camMatrix; // proj * view
uniform mat4 model;

void main()
{
   currentPos = vec3(model * vec4(aPos, 1.0f));

   color = aColor;

   gl_Position = camMatrix * vec4(currentPos, 1.0);
}
