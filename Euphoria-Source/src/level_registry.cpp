#include "level_registry.hpp"
#include <iostream>

enum class Tiles {
    SOLID = 0,
    INTERACTABLE = 1,
};

void loadLevel(LEVEL l) {
    // clear level elements
    while (registry.levelElements.entities.size() > 0)
        registry.remove_all_components_of(registry.levelElements.entities.back());
    // load level data

    Level& level = levels.get(l);

    // temp: load geometry file
    loadGeometryFile(level_path(level.directory + "/geometry.png"));
    // load connects
    for (TransitionData& connect : level.connects) {
        createTransition(connect);
    }
}

void transitionTo(Transition to) {
    vec2 targetPos = to.targetPosition;

    Entity& p = registry.players.entities[0];
    registry.motions.get(p).position = targetPos;

    loadLevel(to.targetLevel);

    Level& level = levels.get(to.targetLevel);
    level.startPos = targetPos;

    GameManager& gm = registry.gameManagers.components[0];
    gm.current_level = to.targetLevel;
}

void loadGeometryFile(std::string file_path) {
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
                    createSolid({ xTo, yTo }, { TILE_SIZE, TILE_SIZE });
                }
                break;
            case Tiles::INTERACTABLE:
                if (opacity == 255) {
                    createItem({ xTo, yTo }, { TILE_SIZE, TILE_SIZE }, { 1.f, 1.f, }, true);
                }
                break;

            }
        }
    }

    // camera bounds
    registry.gameManagers.components[0].bounds = { TILE_SIZE * im_width, TILE_SIZE * im_height };

    stbi_image_free(imageData);
}

LevelRegistry levels;