#version 430 core
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
void main() {
	gl_Position = P * V * M * vec4(vertex_position, 1.0);
}
