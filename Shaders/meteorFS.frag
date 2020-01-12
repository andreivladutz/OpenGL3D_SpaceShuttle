// Shader-ul de fragment / Fragment shader  
#version 450 core

out vec4 out_Color;

uniform vec3 lightColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float shininess;
    // sampler2D texture_diffuse2;
    // sampler2D texture_diffuse3;
    // sampler2D texture_specular1;
    // sampler2D texture_specular2;
}; 

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
  
uniform Material material;

in VS_OUT {
    vec3 fragPos, lightPosition, viewPosition;
    vec2 texCoord;
	//vec3 normal;
} vs_in;

struct light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

light computeLight(float shininess = 32) {
    light outputLight;

    // the normal is now given by the normal texture
    vec3 Normal = texture(texture_normal1, vs_in.texCoord).rgb;
    // transform normal vector components from range [0, 1] to range [-1,1]
    Normal = normalize(Normal * 2.0 - 1.0);

    float ambientStrength = 0.3, specularStrength = 0.5;
    outputLight.ambient = ambientStrength * lightColor;

    vec3 lightDirection = normalize(vs_in.lightPosition - vs_in.fragPos);

    // diffuse light
    float diff = max(dot(lightDirection, Normal), 0);
    outputLight.diffuse = diff * lightColor;

    lightDirection = -lightDirection;
    vec3 reflectedLightDir = reflect(lightDirection, Normal);

    vec3 viewDirection = normalize(vs_in.viewPosition - vs_in.fragPos);
    // the specular component is affected by the angle between view direction and the reflected light direction
    float spec = pow(max(dot(reflectedLightDir, viewDirection), 0), shininess);
    outputLight.specular = spec * specularStrength * lightColor;

    return outputLight;
}

// compute light for applying it to texture coordinates
vec4 addLighting() {
    light computedLight = computeLight();
    vec3 ambient = computedLight.ambient,
        diffuse = computedLight.diffuse,
        specular = computedLight.specular;

    return vec4((ambient + diffuse + specular), 1);
}

// compute light taking into account the material properties
vec4 addLightingToMaterial() {
    light computedLight = computeLight();
    vec3 ambient = computedLight.ambient * material.ambient,
        diffuse = computedLight.diffuse * material.diffuse,
        specular = computedLight.specular * material.specular;

    return vec4((ambient + diffuse + specular), 1);
}

void main(void) {
    // if we have texture coordinates then we should also have textures
    if (vs_in.texCoord != vec2(0, 0)) {
        vec4 texturedFrag =  texture2D(texture_diffuse1, vs_in.texCoord);
        //vec4 texturedFrag =  texture(texture_normal1, vs_in.texCoord);
            //texture(texture_diffuse3, vs_in.texCoord)
            //texture(texture_specular1, vs_in.texCoord)
            //texture(texture_specular2, vs_in.texCoord)
        
        out_Color = addLighting() * texturedFrag;
    }
    else {
        out_Color = addLightingToMaterial();
    }

}