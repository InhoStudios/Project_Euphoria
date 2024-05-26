#include "level_registry.hpp"
#include <iostream>

enum class Tiles {
    SOLID = 0,
    INTERACTABLE = 1,
};

void loadLevel() {

}

void loadGeometryFile(RenderSystem* renderer, std::string file_path) {
    int im_width, im_height, num_channels;
    uint8_t* imageData = stbi_load(file_path.c_str(), &im_width, &im_height, &num_channels, 0);

    for (int yy = 0; yy < im_height; yy++) {
        for (int xx = 0; xx < im_width; xx++) {
            Tiles tile = (Tiles) imageData[num_channels * (yy * im_width + xx)];
            uint8_t opacity = imageData[num_channels * (yy * im_width + xx) + 1];
            int xTo = xx * TILE_SIZE, yTo = yy * TILE_SIZE;

            // create different tiles
            switch (tile) {
            case Tiles::SOLID:
                if (opacity == 255) {
                    createSolid(renderer, { xTo, yTo }, { TILE_SIZE, TILE_SIZE });
                }
                break;
            case Tiles::INTERACTABLE:
                if (opacity == 255) {
                    createItem(renderer, { xTo, yTo }, { TILE_SIZE, TILE_SIZE }, { 1.f, 1.f, }, true);
                }
                break;

            }
        }
    }

    stbi_image_free(imageData);
}

LevelRegistry levels;