#include <graphics/gui/scene_inspector.h>
#include <scene/components/transform.h>
#include <scene/components/model.h>
#include <scene/components/camera.h>
#include <scene/components/light.h>
#include <scene/components/script.h>

static entity_t* gui_entity = NULL;

void gui_component_inspector_set_entity(entity_t* entity) {
    gui_entity = entity;
}

void gui_transform(struct nk_context* nk_context, entity_t* entity) {
    if (nk_tree_push(nk_context, NK_TREE_NODE, "transform", NK_MAXIMIZED)) {
        transform_component_t* transform = (transform_component_t*)entity_get_component(entity, ENTITY_COMPONENT_TRANSFORM);

        nk_layout_row_dynamic(nk_context, 16, 4);

        nk_label(nk_context, "position:", NK_TEXT_ALIGN_LEFT);
        nk_property_float(nk_context, "#x", -FLT_MAX, &(transform->pos[0]), FLT_MAX, 1.0f, 0.01f);
        nk_property_float(nk_context, "#y", -FLT_MAX, &(transform->pos[1]), FLT_MAX, 1.0f, 0.01f);
        nk_property_float(nk_context, "#z", -FLT_MAX, &(transform->pos[2]), FLT_MAX, 1.0f, 0.01f);

        nk_label(nk_context, "rotation:", NK_TEXT_ALIGN_LEFT);
        nk_property_float(nk_context, "#x", -FLT_MAX, &(transform->rotation[0]), FLT_MAX, 1.0f, 0.01f);
        nk_property_float(nk_context, "#y", -FLT_MAX, &(transform->rotation[1]), FLT_MAX, 1.0f, 0.01f);
        nk_property_float(nk_context, "#z", -FLT_MAX, &(transform->rotation[2]), FLT_MAX, 1.0f, 0.01f);

        nk_label(nk_context, "scale:", NK_TEXT_ALIGN_LEFT);
        nk_property_float(nk_context, "#x", -FLT_MAX, &(transform->scale[0]), FLT_MAX, 1.0f, 0.01f);
        nk_property_float(nk_context, "#y", -FLT_MAX, &(transform->scale[1]), FLT_MAX, 1.0f, 0.01f);
        nk_property_float(nk_context, "#z", -FLT_MAX, &(transform->scale[2]), FLT_MAX, 1.0f, 0.01f);
        
        nk_tree_pop(nk_context);
    }
}

void gui_model(struct nk_context* nk_context, entity_t* entity) {
    if (nk_tree_push(nk_context, NK_TREE_NODE, "model", NK_MAXIMIZED)) {
        model_component_t* model_comp = (model_component_t*)entity_get_component(entity, ENTITY_COMPONENT_MODEL);
        model_t* model = model_comp->model;
        material_t* materials = model_get_materials(model);

        nk_layout_row_dynamic(nk_context, 16, 1);
        model_comp->draw = nk_check_label(nk_context, "draw", model_comp->draw);

        nk_tree_pop(nk_context);
    }
}

void gui_camera(struct nk_context* nk_context, entity_t* entity) {
    if (nk_tree_push(nk_context, NK_TREE_NODE, "camera", NK_MAXIMIZED)) {
        camera_component_t* camera = (camera_component_t*)entity_get_component(entity, ENTITY_COMPONENT_CAMERA);

        nk_layout_row_dynamic(nk_context, 16, 1);

        camera->use = nk_check_label(nk_context, "use", camera->use);
        
        nk_tree_pop(nk_context);
    }
}

void gui_light(struct nk_context* nk_context, entity_t* entity) {
    if (nk_tree_push(nk_context, NK_TREE_NODE, "light", NK_MAXIMIZED)) {
        light_component_t* light = (light_component_t*)entity_get_component(entity, ENTITY_COMPONENT_LIGHT);

        const char* light_types[] = {
            "directional",
            "point",
            "spot"
        };

        nk_layout_row_dynamic(nk_context, 16, 1);
        light->cast = nk_check_label(nk_context, "cast", light->cast);

        nk_layout_row_dynamic(nk_context, 16, 2);
        nk_label(nk_context, "type:", NK_TEXT_ALIGN_LEFT);
        light->light.type = nk_combo(nk_context, light_types, 3, light->light.type, 16, nk_vec2(200, 200));

        nk_layout_row_dynamic(nk_context, 16, 2);
        nk_label(nk_context, "ambient", NK_TEXT_ALIGN_LEFT);
        if (nk_combo_begin_color(nk_context, gui_nk_colorf_to_nk_color(gui_vec3_to_nk_color(light->light.ambient)), nk_vec2(nk_widget_width(nk_context), 400))) {
		    nk_layout_row_dynamic(nk_context, 120, 1);
	    	gui_nk_color_to_vec3(nk_color_picker(nk_context, gui_vec3_to_nk_color(light->light.ambient), NK_RGB), light->light.ambient);
	    	nk_combo_end(nk_context);
	    }

        nk_layout_row_dynamic(nk_context, 16, 2);
        nk_label(nk_context, "diffuse", NK_TEXT_ALIGN_LEFT);
        if (nk_combo_begin_color(nk_context, gui_nk_colorf_to_nk_color(gui_vec3_to_nk_color(light->light.diffuse)), nk_vec2(nk_widget_width(nk_context), 400))) {
		    nk_layout_row_dynamic(nk_context, 120, 1);
	    	gui_nk_color_to_vec3(nk_color_picker(nk_context, gui_vec3_to_nk_color(light->light.diffuse), NK_RGB), light->light.diffuse);
	    	nk_combo_end(nk_context);
	    }


        nk_layout_row_dynamic(nk_context, 16, 2);
        nk_label(nk_context, "specular", NK_TEXT_ALIGN_LEFT);
        if (nk_combo_begin_color(nk_context, gui_nk_colorf_to_nk_color(gui_vec3_to_nk_color(light->light.specular)), nk_vec2(nk_widget_width(nk_context), 400))) {
		    nk_layout_row_dynamic(nk_context, 120, 1);
	    	gui_nk_color_to_vec3(nk_color_picker(nk_context, gui_vec3_to_nk_color(light->light.specular), NK_RGB), light->light.specular);
	    	nk_combo_end(nk_context);
	    }

        nk_layout_row_dynamic(nk_context, 16, 1);
        if (light->light.type == LIGHT_TYPE_POINT || light->light.type == LIGHT_TYPE_SPOT) {
            nk_property_float(nk_context, "constant", 0.0f, &light->light.constant, FLT_MAX, 1.0f, 0.1f);
            nk_property_float(nk_context, "linear", 0.0f, &light->light.linear, FLT_MAX, 1.0f, 0.1f);
            nk_property_float(nk_context, "quadratic", 0.0f, &light->light.quadratic, FLT_MAX, 1.0f, 0.1f);
        }

        if (light->light.type == LIGHT_TYPE_SPOT) {
            nk_property_float(nk_context, "cutoff", 0.0f, &light->light.cutoff, FLT_MAX, 1.0f, 0.1f);
            nk_property_float(nk_context, "outer cutoff", 0.0f, &light->light.outer_cutoff, FLT_MAX, 1.0f, 0.1f);
        }

        nk_tree_pop(nk_context);
    }
}

void gui_script(struct nk_context* nk_context, entity_t* entity) {
    if (nk_tree_push(nk_context, NK_TREE_NODE, "script", NK_MAXIMIZED)) {
        script_component_t* script = (script_component_t*)entity_get_component(entity, ENTITY_COMPONENT_SCRIPT);

        nk_layout_row_dynamic(nk_context, 16, 1);

        script->run = nk_check_label(nk_context, "run", script->run);
        if (nk_button_label(nk_context, "restart")) {
            script->has_started = false;
        }
        
        nk_tree_pop(nk_context);
    }
}

void gui_component_inspector(struct nk_context* nk_context, void* user_data) {
    if (gui_entity == NULL) {
        return;
    }

    if (nk_begin(nk_context, "component inspector", nk_rect(10, 540, 300, 300), gui_default_window_flags)) {
        if (entity_has_component(gui_entity, ENTITY_COMPONENT_TRANSFORM)) {
            gui_transform(nk_context, gui_entity);
        }

        if (entity_has_component(gui_entity, ENTITY_COMPONENT_MODEL)) {
            gui_model(nk_context, gui_entity);
        }

        if (entity_has_component(gui_entity, ENTITY_COMPONENT_CAMERA)) {
            gui_camera(nk_context, gui_entity);
        }

        if (entity_has_component(gui_entity, ENTITY_COMPONENT_LIGHT)) {
            gui_light(nk_context, gui_entity);
        }

        if (entity_has_component(gui_entity, ENTITY_COMPONENT_SCRIPT)) {
            gui_script(nk_context, gui_entity);
        }
    }
    nk_end(nk_context);
}
