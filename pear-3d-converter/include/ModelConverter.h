#pragma once
#include <pear-3d.h>
#include <assimp/scene.h>
#include <vector>
#include <string>

class ModelConverter {
public:
    ModelConverter(const std::string& filename);
    Pear3D getPear3D();

private:
    void processNode(aiNode* node);
    void processMesh(aiMesh* mesh);

    Pear3D pearModel;
    std::vector<Pear3D_Mesh> meshes;
    std::vector<Pear3D_Material> materials;

    const aiScene* scene;
    std::string directory;
    bool okay = true;
};
