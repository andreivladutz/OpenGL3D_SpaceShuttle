#pragma once
#include "../glm/glm.hpp"
#include "string"
#include "vector"

#include "ProgramLoader.h"
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;

class Model
{
public:
    /*  Functions   */
    Model(const char* path)
    {
        loadModel(path);
    }
    void Draw(ProgramLoader* program);
private:
    vector<Texture> textures_loaded;

    /*  Model Data  */
    vector<Mesh> meshes;
    string directory;
    /*  Functions   */
    void loadModel(string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        string typeName);
    Material processMaterial(aiMaterial* mat);
};

unsigned int TextureFromFile(const char* path, const string& directory);