#include <pear-3d.h> 
#include <vector>

int main(int argc, char* argv[]) {
    std::vector<Vertex> vertices = {
        { .position = {0.0f, 0.0f, 0.0f}, .texture_coords = { 0.0f, 0.0f }, .normal = { 0.0f, 0.0f, 0.0f } },
        { .position = {1.0f, 0.0f, 0.0f}, .texture_coords = { 0.0f, 0.0f }, .normal = { 0.0f, 0.0f, 0.0f } },
        { .position = {2.0f, 0.0f, 0.0f}, .texture_coords = { 0.0f, 0.0f }, .normal = { 0.0f, 0.0f, 0.0f } },
    };

    std::vector<Material> materials = {
        { .texture_diffuse = "diffuse.jpg", .texture_specular = "specular.jpg", .texture_normal = "normal.jpg", .color = { 1.0f, 0.0f, 1.0f, 1.0f } }
    };

    Mesh mesh;
    mesh.material_index = 0;
    mesh.num_faces = 1;
    mesh.faces = new Face[1];
    mesh.faces[0].vertex_indices[0] = 0;
    mesh.faces[0].vertex_indices[1] = 1;
    mesh.faces[0].vertex_indices[2] = 2;
    std::vector<Mesh> meshes = { mesh };


    Pear3D model;
    model.vertices = vertices.data();
    model.num_vertices = 3;
    model.materials = materials.data();
    model.num_materials = 1;
    model.meshes = meshes.data();
    model.num_meshes = 1;

    pear3d_save("model.pear3d", model);

    bool success;
    Pear3D loaded_model = pear3d_load("model.pear3d", &success);

    return 0;
}
