#pragma once
#include "../glm/glm.hpp"
#include "string"
#include "vector"

#include "ProgramLoader.h"

using namespace std;

const GLuint vertPositionAttrib = 0,
    vertNormalAttrib = 1,
    vertTexCoordAttrib = 2,
    vertTangentAttrib = 4,
    vertBitangentAttrib = 5;

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    // coords for normal mapping
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float shininess;
};

class Mesh {
public:
    unsigned int VAO;

    /*  Mesh Data  */
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    Material material;

    /*  Functions  */
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, Material material);
    void Draw(ProgramLoader* program);
    void DrawPoints(ProgramLoader* program);
private:
    /*  Render data  */
    unsigned int VBO, EBO;
    /*  Functions    */
    void setupMesh();
};