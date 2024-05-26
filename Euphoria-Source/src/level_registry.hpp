#pragma once

#include "tiny_ecs_registry.hpp"
#include "common.hpp"
#include "world_init.hpp"

void loadLevel();

void loadGeometryFile(RenderSystem* renderer, std::string file_path);

class LevelRegistry
{
public:
    std::vector<Level*> levels;

    Level tut_int_1, tut_int_2, tut_int_3, tut_int_4, tut_ext_1, tut_ext_2;

    LevelRegistry() {
        this->initLevels();

        levels = {
            &tut_int_1,
            &tut_int_2,
            &tut_int_3,
            &tut_int_4,
            &tut_ext_1,
            &tut_ext_2,


        };
    }

    Level& get(int i) {
        return *levels[i];
    }
    //
private:
    void initLevels() {
        init_tutorial_level();
    }

    void init_tutorial_level() {
        // init tut_int_1, 2, 3, and ext_1, 2
    }
};

extern LevelRegistry levels;