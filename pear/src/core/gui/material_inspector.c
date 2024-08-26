#include <core/gui/material_inspector.h>
#include <graphics/material.h>

void gui_material_inspector(struct nk_context* nk_context, void* user_data) {
    if (nk_begin(nk_context, "material inspector", nk_rect(0.0f, 0.0f, 250.0f, 250.0f), gui_default_window_flags)) {
        for (u32 i = 0; i < material_get_num_materials(); i++) {
            Material* material = material_get(i);

            char tree_name[64];
            sprintf(tree_name, "material %d (0x%x)", i, material);

            if (nk_tree_push_id(nk_context, NK_TREE_TAB, tree_name, NK_MINIMIZED, i)) {
                struct nk_colorf color;

                nk_layout_row_dynamic(nk_context, 16, 1);
                nk_labelf(nk_context, NK_TEXT_ALIGN_LEFT, "albedo texture: 0x%x", material->texture_albedo);
                nk_labelf(nk_context, NK_TEXT_ALIGN_LEFT, "specular texture: 0x%x", material->texture_specular);
                nk_labelf(nk_context, NK_TEXT_ALIGN_LEFT, "normal texture: 0x%x", material->texture_normal);

                nk_layout_row_dynamic(nk_context, 16, 1);
                nk_label(nk_context, "diffuse color:", NK_TEXT_ALIGN_LEFT);
                nk_layout_row_dynamic(nk_context, 128, 1);
                color = gui_vec4_to_nk_color(material->color_diffuse);
                gui_nk_color_to_vec4(nk_color_picker(nk_context, color, NK_RGBA), material->color_diffuse);

                nk_layout_row_dynamic(nk_context, 16, 1);
                nk_label(nk_context, "specular color:", NK_TEXT_ALIGN_LEFT);
                nk_layout_row_dynamic(nk_context, 128, 1);
                color = gui_vec4_to_nk_color(material->color_specular);
                gui_nk_color_to_vec4(nk_color_picker(nk_context, color, NK_RGBA), material->color_specular);

                nk_layout_row_dynamic(nk_context, 16, 1);
                nk_checkbox_label(nk_context, "force use color", (nk_bool*)&material->use_color);

                nk_tree_pop(nk_context);
            }
        }
    }
    nk_end(nk_context);
}
