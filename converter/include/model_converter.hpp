#pragma once

#include <pear_model.h>
#include <assimp/scene.h>
#include <vector>
#include <string>

class ModelConverter {
public:
    ModelConverter(const std::string& filename);
    ~ModelConverter();
    pear_model_t get_pear_model();

private:
    void process_node(aiNode* node);
    pear_mesh_t process_mesh(aiMesh* mesh);
    pear_material_t process_material(aiMaterial* mat);
    std::string process_texture(aiMaterial* mat, aiTextureType type);

private:
    std::string directory;
    const aiScene* scene;
    std::vector<pear_mesh_t> meshes;
    std::vector<std::string> loaded_textures;
    pear_model_t model;
};
