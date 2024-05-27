#pragma once

#include "tiny_ecs_registry.hpp"
#include "common.hpp"
#include "world_init.hpp"

void loadLevel(RenderSystem* renderer, LEVEL l);

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

    Level& get(LEVEL i) {
        return *levels[(int) i];
    }
    //
private:
    void initLevels() {
        init_tutorial_level();
    }

    void init_tutorial_level() {
        // init tut_int_1, 2, 3, and ext_1, 2
        tut_int_1.directory = "tut_int_1";
        tut_int_1.startPos = { 112, 264 };


        tut_int_2.directory = "tut_int_2";
        tut_int_3.directory = "tut_int_3";
        tut_int_4.directory = "tut_int_4";

        tut_ext_1.directory = "tut_ext_1";
        tut_ext_2.directory = "tut_ext_2";
    }
};

extern LevelRegistry levels;