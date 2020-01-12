#version 450 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_Tex_Coords;
// leave one out for the compatibility with
// the other shaders which have a color attribute
layout(location = 4) in vec3 in_Tangent;
layout(location = 5) in vec3 in_Bitangent;

uniform vec3 lightPosition;
uniform vec3 viewPos;
uniform mat4 projectionMatrix, viewMatrix, modelMatrix;

out VS_OUT {
    vec3 fragPos, lightPosition, viewPosition;
	vec2 texCoord;
} vs_out;


void main(void) {
	mat4 modelViewMatrix = viewMatrix * modelMatrix;

	gl_Position = projectionMatrix * modelViewMatrix * vec4(in_Position, 1);

	// set the matrix to a 3 x 3 so translations are excluded
	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));

	vec3 T = normalize(normalMatrix * in_Tangent);
    vec3 N = normalize(normalMatrix * in_Normal);
    vec3 B = normalize(normalMatrix * in_Bitangent);
	
    // TBN must form a right handed coord system.
    // Some models have symetric UVs. Check and fix.
    if (dot(cross(N, T), B) < 0.0) {
		T = T * -1.0;
	}
	// get the inverse of the TBN matrix
	mat3 TBN = transpose(mat3(T, B, N));

	// the position of the fragment that gets illuminated
	vs_out.fragPos = TBN * (modelMatrix * vec4(in_Position, 1)).xyz;
	vs_out.lightPosition = TBN * lightPosition;
	vs_out.viewPosition = TBN * viewPos;

	vs_out.texCoord = in_Tex_Coords;
} 
 