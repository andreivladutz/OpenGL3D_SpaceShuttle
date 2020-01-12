#version 450 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_Tex_Coords;


uniform mat4 projectionMatrix, viewMatrix, modelMatrix;

out VS_OUT {
	vec2 texCoord;
} vs_out;


void main(void) {
	mat4 modelViewMatrix = viewMatrix * modelMatrix;

	gl_Position = projectionMatrix * modelViewMatrix * vec4(in_Position, 1);

	vs_out.texCoord = in_Tex_Coords;
} 
 