#version 330 core

layout (location = 3) in vec4 vertex; // <vec2 pos, vec2 tex>

out vec2 texCoord;

uniform mat4 projection;

void main()
{
	// OpenGL uses left-hand rule, so text is rendered in the opposite direction
	// the Z-index of the text must be negative
	gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
	gl_Position.z = -1.0f; 
	texCoord = vertex.zw;
}
