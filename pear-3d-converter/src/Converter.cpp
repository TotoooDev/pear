#include <Converter.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

Converter::Converter(const std::string& filename) {
    Assimp::Importer importer;
    this->scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
    if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr) {
        std::cout << importer.GetErrorString() << std::endl;
        this->okay = false;
        return;
    }

    this->directory = filename.substr(0, filename.find_last_of('/'));
    this->processNode(this->scene->mRootNode);
}

Pear3D Converter::getPear3D() {
    this->pearModel.meshes = this->meshes.data();
    this->pearModel.materials = this->materials.data();
    this->pearModel.num_meshes = this->meshes.size();
    this->pearModel.num_materials = this->materials.size();
    return this->pearModel;
}

void Converter::processNode(aiNode* node) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = this->scene->mMeshes[node->mMeshes[i]];
        this->processMesh(mesh);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        this->processNode(node->mChildren[i]);
    }
}

void Converter::processMesh(aiMesh* mesh) {
    std::vector<Pear3D_Vertex> vertices;
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Pear3D_Vertex vertex;

        vertex.position[0] = mesh->mVertices[i].x;
        vertex.position[1] = mesh->mVertices[i].y;
        vertex.position[2] = mesh->mVertices[i].z;

        // std::cout << vertex.position[0] << " " << vertex.position[1] << " " << vertex.position[2] << std::endl;

        vertex.normal[0] = mesh->mNormals[i].x;
        vertex.normal[1] = mesh->mNormals[i].y;
        vertex.normal[2] = mesh->mNormals[i].z;

        if (mesh->mTextureCoords[0] != nullptr) {
            vertex.texture_coords[0] = mesh->mTextureCoords[0][i].x;
            vertex.texture_coords[1] = mesh->mTextureCoords[0][i].y;
        }
        else {
            vertex.texture_coords[0] = 0.0f;
            vertex.texture_coords[1] = 0.0f;
        }

        vertices.push_back(vertex);
    }

    std::vector<uint32_t> indices;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            std::cout << face.mIndices[j] << std::endl;
            indices.push_back(face.mIndices[j]);
        }
    }

    uint32_t material_index;
    for (uint32_t i = 0; i < this->scene->mMaterials[mesh->mMaterialIndex]->GetTextureCount(aiTextureType_DIFFUSE); i++) {
        aiString str;
        this->scene->mMaterials[mesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, i, &str);
        bool skip = false;
        uint32_t index = 0;
        for (Pear3D_Material mat : this->materials) {
            if (std::strcmp(mat.texture_diffuse, str.C_Str()) == 0) {
                material_index = index;
                skip = true;
                break;
            }
            index++;
        }
        if (!skip) {
            Pear3D_Material mat;
            mat.texture_diffuse = (char*)malloc(sizeof(char) * str.length + 1);
            mat.texture_specular = "";
            mat.texture_normal = "";
            mat.color[0] = 1.0f;
            mat.color[1] = 0.0f;
            mat.color[2] = 1.0f;
            mat.color[3] = 1.0f;

            std::strcpy((char*)mat.texture_diffuse, str.C_Str());
            this->materials.push_back(mat);
            material_index = index;
        }
    }

    Pear3D_Mesh pearMesh;

    pearMesh.vertices = (Pear3D_Vertex*)malloc(sizeof(Pear3D_Vertex) * vertices.size());
    for (unsigned int i = 0; i < vertices.size(); i++) {
        pearMesh.vertices[i] = vertices[i];
    }

    pearMesh.indices = (uint32_t*)malloc(sizeof(uint32_t) * indices.size());
    for (unsigned int i = 0; i < indices.size(); i++) {
        pearMesh.indices[i] = indices[i];
    }

    pearMesh.num_vertices = vertices.size();
    pearMesh.num_indices = indices.size();
    pearMesh.material_index = material_index;

    this->meshes.push_back(pearMesh);
}
