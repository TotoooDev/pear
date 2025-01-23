#include <loaders/skybox.h>
#include <loaders/image.h>
#include <util/filesystem.h>
#include <string.h>

cubemap_t* loader_load_skybox(const char* directory) {
    image_t* images[6];
    
    char right_path[1024];
    strcpy(right_path, directory);
    strcat(right_path, "/right.jpg");
    images[0] = loader_load_image(right_path);

    char left_path[1024];
    strcpy(left_path, directory);
    strcat(left_path, "/left.jpg");
    images[1] = loader_load_image(left_path);

    char top_path[1024];
    strcpy(top_path, directory);
    strcat(top_path, "/top.jpg");
    images[2] = loader_load_image(top_path);

    char bottom_path[1024];
    strcpy(bottom_path, directory);
    strcat(bottom_path, "/bottom.jpg");
    images[3] = loader_load_image(bottom_path);

    char front_path[1024];
    strcpy(front_path, directory);
    strcat(front_path, "/front.jpg");
    images[4] = loader_load_image(front_path);

    char back_path[1024];
    strcpy(back_path, directory);
    strcat(back_path, "/back.jpg");
    images[5] = loader_load_image(back_path);

    cubemap_t* cubemap = cubemap_new_from_images(images, TEXTURE_WRAPPING_CLAMP, TEXTURE_FILTERING_LINEAR);
    return cubemap;
}
