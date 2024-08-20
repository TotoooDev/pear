#include <core/gui/material_inspector.h>
#include <graphics/material.h>

struct nk_colorf gui_vec4_to_nk_color(vec4 color) {
    struct nk_colorf nk_color = {
        .r = color[0],
        .g = color[1],
        .b = color[2],
        .a = color[3]
    };
    return nk_color;
}

void gui_nk_color_to_vec4(struct nk_colorf nk_color, vec4 color) {
    color[0] = nk_color.r;
    color[1] = nk_color.g;
    color[2] = nk_color.b;
    color[3] = nk_color.a;
}

void gui_material_inspector(struct nk_context* nk_context, void* user_data) {
    if (nk_begin(nk_context, "material inspector", nk_rect(0.0f, 0.0f, 250.0f, 250.0f), gui_default_window_flags)) {
        for (u32 i = 0; i < material_get_num_materials(); i++) {
            Material* material = material_get(i);

            char tree_name[64];
            sprintf(tree_name, "material %d (0x%x)", i, material);

            if (nk_tree_push(nk_context, NK_TREE_NODE, tree_name, NK_MINIMIZED)) {
                nk_layout_row_dynamic(nk_context, 16, 1);

                nk_labelf(nk_context, NK_TEXT_ALIGN_LEFT, "albedo texture: 0x%x", material->texture_albedo);
                nk_labelf(nk_context, NK_TEXT_ALIGN_LEFT, "specular texture: 0x%x", material->texture_specular);
                nk_labelf(nk_context, NK_TEXT_ALIGN_LEFT, "normal texture: 0x%x", material->texture_normal);

                nk_layout_row_dynamic(nk_context, 16, 1);
                nk_label(nk_context, "diffuse color:", NK_TEXT_ALIGN_LEFT);
                nk_layout_row_dynamic(nk_context, 128, 1);
                struct nk_colorf color = gui_vec4_to_nk_color(material->color_diffuse);
                gui_nk_color_to_vec4(nk_color_picker(nk_context, color, NK_RGBA), material->color_diffuse);

                nk_layout_row_dynamic(nk_context, 16, 1);
                nk_label(nk_context, "specular color:", NK_TEXT_ALIGN_LEFT);
                nk_layout_row_dynamic(nk_context, 128, 1);
                color = gui_vec4_to_nk_color(material->color_specular);
                gui_nk_color_to_vec4(nk_color_picker(nk_context, color, NK_RGBA), material->color_specular);

                nk_tree_pop(nk_context);
            }
        }
    }
    nk_end(nk_context);
}
