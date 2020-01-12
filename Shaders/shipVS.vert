#version 450 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_Tex_Coords;

layout(location = 3) in vec3 in_Color;

uniform vec3 lightPosition;
uniform mat4 projectionMatrix, viewMatrix, modelMatrix;

out VS_OUT {
	vec3 color, normal, fragPos, lightPosition;
	vec2 texCoord;
} vs_out;


void main(void) {
	mat4 modelViewMatrix = viewMatrix * modelMatrix;

	gl_Position = projectionMatrix * modelViewMatrix * vec4(in_Position, 1);

	vs_out.color = in_Color;

	// set the w component to 0 so it doesn't get affected by translations
	vec4 Normal = vec4(in_Normal, 0);
	Normal = normalize(transpose(inverse(modelViewMatrix)) * Normal);

	vs_out.normal = Normal.xyz;
	// the position of the fragment that gets illuminated
	vs_out.fragPos = (modelViewMatrix * vec4(in_Position, 1)).xyz;
	vs_out.lightPosition = (viewMatrix * vec4(lightPosition, 1)).xyz;

	vs_out.texCoord = in_Tex_Coords;
} 
 