#include <model_converter.hpp>
#include <image_converter.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <iostream>

ModelConverter::ModelConverter(const std::string& filename) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
    if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr) {
        std::cout << "error: " << importer.GetErrorString() << std::endl;
        return;
    }

    this->scene = scene;
    this->directory = filename.substr(0, filename.find_last_of('/'));
    this->process_node(scene->mRootNode);

    this->model.num_meshes = this->meshes.size();
    this->model.meshes = new pear_mesh_t[this->meshes.size()];
    memcpy(this->model.meshes, this->meshes.data(), sizeof(pear_mesh_t) * this->model.num_meshes);

    std::vector<pear_material_t> materials;
    for (uint32_t i = 0; i < this->scene->mNumMaterials; i++) {
        pear_material_t material = this->process_material(this->scene->mMaterials[i]);
        materials.push_back(material);
    }

    this->model.num_materials = materials.size();
    this->model.materials = new pear_material_t[materials.size()];
    memcpy(this->model.materials, materials.data(), sizeof(pear_material_t) * this->model.num_materials);
}

ModelConverter::~ModelConverter() {
    pear_model_free(&this->model);
}

pear_model_t ModelConverter::get_pear_model() {
    return model;
}

void ModelConverter::process_node(aiNode* node) {
    for (uint32_t i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = this->scene->mMeshes[node->mMeshes[i]];
        pear_mesh_t pear_mesh = this->process_mesh(mesh);
        this->meshes.push_back(pear_mesh);
    }

    for (uint32_t i = 0; i < node->mNumChildren; i++) {
        this->process_node(node->mChildren[i]);
    }
}

pear_mesh_t ModelConverter::process_mesh(aiMesh* mesh) {
    std::vector<pear_vertex_t> vertices;
    std::vector<uint32_t> indices;

    for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
        pear_vertex_t vertex;

        vertex.pos[0] = mesh->mVertices[i].x;
        vertex.pos[1] = mesh->mVertices[i].y;
        vertex.pos[2] = mesh->mVertices[i].z;

        if (mesh->HasNormals()) {
            vertex.normal[0] = mesh->mNormals[i].x;
            vertex.normal[1] = mesh->mNormals[i].y;
            vertex.normal[2] = mesh->mNormals[i].z;
        }
        else {
            glm_vec3_zero(vertex.normal);
        }

        if (mesh->mTextureCoords[0]) {
            vertex.texture_coords[0] = mesh->mTextureCoords[0][i].x;
            vertex.texture_coords[1] = mesh->mTextureCoords[0][i].y;
        }
        else {
            glm_vec2_zero(vertex.texture_coords);
        }

        vertices.push_back(vertex);
    }

    for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (uint32_t j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    pear_mesh_t pear_mesh;
    pear_mesh.num_vertices = vertices.size();
    pear_mesh.num_indices = indices.size();
    pear_mesh.material_index = mesh->mMaterialIndex;
    pear_mesh.vertices = new pear_vertex_t[pear_mesh.num_vertices];
    pear_mesh.indices = new uint32_t[pear_mesh.num_indices];

    memcpy(pear_mesh.vertices, vertices.data(), sizeof(pear_vertex_t) * pear_mesh.num_vertices);
    memcpy(pear_mesh.indices, indices.data(), sizeof(uint32_t) * pear_mesh.num_indices);

    return pear_mesh;
}

pear_material_t ModelConverter::process_material(aiMaterial* mat) {
    pear_material_t material;

    std::string diffuse = this->process_texture(mat, aiTextureType_DIFFUSE);
    std::string specular = this->process_texture(mat, aiTextureType_SPECULAR);
    std::string normal = this->process_texture(mat, aiTextureType_HEIGHT);

    material.diffuse_path = new char[diffuse.length() + 1];
    material.specular_path = new char[specular.length() + 1];
    material.normal_path = new char[normal.length() + 1];

    strcpy(material.diffuse_path, diffuse.c_str());
    strcpy(material.specular_path, specular.c_str());
    strcpy(material.normal_path, normal.c_str());

    aiColor3D diffuse_color;
    if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse_color) == AI_SUCCESS) {
        material.color[0] = diffuse_color.r;
        material.color[1] = diffuse_color.g;
        material.color[2] = diffuse_color.b;
    }
    else {
        material.color[0] = 1.0f;
        material.color[1] = 0.0f;
        material.color[2] = 1.0f;
    }

    return material;
}

std::string ModelConverter::process_texture(aiMaterial* mat, aiTextureType type) {
    if (mat->GetTextureCount(type) == 0) {
        return "";
    }

    aiString filename;
    mat->GetTexture(type, 0, &filename);

    std::string converted_filename = std::string(filename.C_Str()).append(".image");

    for (uint32_t i = 0; i < this->loaded_textures.size(); i++) {
        if (converted_filename == this->loaded_textures[i]) {
            return this->loaded_textures[i];
        }
    }

    ImageConverter image_converter(filename.C_Str());
    pear_image_t image = image_converter.get_pear_image();
    pear_image_write(image, converted_filename.c_str());

    this->loaded_textures.push_back(converted_filename);
    return converted_filename;
}
