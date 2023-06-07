#version 330 core
layout (location = 0) in vec2 aPos; // postion vector
layout (location = 1) in vec3 aColor; // color of the
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;
uniform mat4 trans;

void main()
{
	gl_Position = trans * vec4(aPos, 0.0f, 1.0);
	ourColor = aColor;
	TexCoord = aTexCoord;
}