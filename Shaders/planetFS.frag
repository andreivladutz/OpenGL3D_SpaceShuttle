// Shader-ul de fragment / Fragment shader  
#version 450 core

out vec4 out_Color;

uniform vec3 lightColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float shininess;
}; 
  
uniform sampler2D texture_diffuse1;
uniform Material material;

in VS_OUT {
    vec2 texCoord;
} vs_in;

vec4 getMaterial() {
    vec3 ambient = vec3(1) * material.ambient,
        diffuse = vec3(1) * material.diffuse,
        specular = vec3(1) * material.specular;

    return vec4((ambient + diffuse + specular), 1);
}

void main(void) {
    // if we have texture coordinates then we should also have textures
    if (vs_in.texCoord != vec2(0, 0)) {
        vec4 texturedFrag =  texture(texture_diffuse1, vs_in.texCoord);
        out_Color = texturedFrag;
    }
    else {
        out_Color = getMaterial();
    }

}