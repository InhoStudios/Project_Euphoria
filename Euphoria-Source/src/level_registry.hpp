#pragma once

#include "tiny_ecs_registry.hpp"
#include "common.hpp"
#include "world_init.hpp"

void loadLevel(LEVEL l);

void transitionTo(Transition to);

void loadGeometryFile(std::string file_path);

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

    TransitionData createConnect(
        LEVEL targetLevel,
        vec2 targetPos,
        vec2 pos,
        vec2 scale,
        bool needsInput,
        TEXTURE_ASSET_ID sprite=TEXTURE_ASSET_ID::DEFAULT
    ) {
        TransitionData t;
        t.targetLevel = targetLevel;
        t.targetPosition = targetPos;
        t.position = pos;
        t.scale = scale;
        t.needsInput = needsInput;
        t.sprite = sprite;

        return t;
    }

    void init_tutorial_level() {
        // init tut_int_1, 2, 3, and ext_1, 2
        tut_int_1.directory = "tut_int_1";
        tut_int_1.startPos = { 112, 264 };

        TransitionData to_int_2 = createConnect(
            LEVEL::TUT_INT_2,
            { 32, 280 },
            { 624, 180 },
            { 16, 360 },
            false,
            TEXTURE_ASSET_ID::DEFAULT
        );
        tut_int_1.connects.push_back(to_int_2);

        tut_int_2.directory = "tut_int_2";
        tut_int_2.startPos = { 32, 280 };

        tut_int_3.directory = "tut_int_3";
        tut_int_4.directory = "tut_int_4";

        tut_ext_1.directory = "tut_ext_1";
        tut_ext_2.directory = "tut_ext_2";
    }
};

extern LevelRegistry levels;