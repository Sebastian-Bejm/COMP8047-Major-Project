#version 330 core

// Positions
layout (location = 0) in vec3 aPos;
// Normals
//layout (location = 1) in vec3 aNormal;
// Colors
layout (location = 1) in vec3 aColor;

// Outputs the current position for the fragment shader
out vec3 currentPos;
// Outputs the normal for the fragment shader
//out vec3 normal;
// Outputs the color for the fragment shader
out vec3 color;

// Model view projection matrices (proj * view * model)
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
   currentPos = vec3(model * vec4(aPos, 1.0f));

   //normal = aNormal;

   color = aColor;

   //gl_Position = proj * view * model * vec4(aPos, 1.0);
   gl_Position = proj * view * vec4(currentPos, 1.0);
}
