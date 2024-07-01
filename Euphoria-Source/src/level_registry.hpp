#pragma once

#include "tiny_ecs_registry.hpp"
#include "common.hpp"
#include "world_init.hpp"

void loadLevel(LEVEL l);

void transitionTo(Transition to);

void loadGeometryFile(std::string file_path);

void loadEntityFile(std::string file_path);

class LevelRegistry
{
public:
    std::vector<Level*> levels;

    Level tut_int_1, tut_int_2, tut_int_3,
        tut_ext_1, tut_ext_2, tut_ext_3, tut_ext_4, tut_ext_5,
        tut_bridge_1, tut_bridge_2;

    LevelRegistry() {
        this->initLevels();

        levels = {
            &tut_int_1,
            &tut_int_2,
            &tut_int_3,

            &tut_ext_1,
            &tut_ext_2,
            &tut_ext_3,
            &tut_ext_4,
            &tut_ext_5,

            &tut_bridge_1,
            &tut_bridge_2,

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
        tut_int_1.directory = "tutorial/tut_int_1";
        tut_int_1.startPos = { 80, 208 };
        tut_int_1.connects.push_back(
            createConnect(
                LEVEL::TUT_EXT_1,
                { 32, 208 },
                { 1264, 200 },
                { 16, 64 },
                false,
                TEXTURE_ASSET_ID::DEFAULT
            )
        );

        tut_ext_1.directory = "tutorial/tut_ext_1";
        tut_ext_1.startPos = { 32, 208 };
        tut_ext_1.connects.push_back(
            createConnect(
                LEVEL::TUT_INT_1,
                { 1232, 208 },
                { 8, 184 },
                { 16, 64 },
                false,
                TEXTURE_ASSET_ID::DEFAULT
            )
        );
        tut_ext_1.connects.push_back(
            createConnect(
                LEVEL::TUT_INT_2,
                { 32, 432 },
                { 632, 184 },
                { 16, 64 },
                false,
                TEXTURE_ASSET_ID::DEFAULT
            )
        );

        tut_int_2.directory = "tutorial/tut_int_2";
        tut_int_2.startPos = { 32, 432 };
        tut_int_2.connects.push_back(
            createConnect(
                LEVEL::TUT_EXT_1,
                { 616, 208 },
                { 8, 424 },
                { 16, 64 },
                false,
                TEXTURE_ASSET_ID::DEFAULT
            )
        );
        tut_int_2.connects.push_back(
            createConnect(
                LEVEL::TUT_EXT_2,
                { 448, 208 },
                { 8, 200 },
                { 16, 64 },
                false,
                TEXTURE_ASSET_ID::DEFAULT
            )
        );
        tut_int_2.connects.push_back(
            createConnect(
                LEVEL::TUT_EXT_3,
                { 32, 216 },
                { 496, 104 },
                { 16, 32 },
                false,
                TEXTURE_ASSET_ID::DEFAULT
            )
        );

        tut_ext_2.directory = "tutorial/tut_ext_2";
        tut_ext_2.startPos = { 448, 208 };
        tut_ext_2.connects.push_back(
            createConnect(
                LEVEL::TUT_INT_2,
                { 32, 216 },
                { 496, 200 },
                { 16, 64 },
                false,
                TEXTURE_ASSET_ID::DEFAULT
            )
        );

        tut_ext_3.directory = "tutorial/tut_ext_3";
        tut_ext_3.startPos = { 32, 216 };
        tut_ext_3.connects.push_back(
            createConnect(
                LEVEL::TUT_INT_2,
                { 464, 104 },
                { 8, 208 },
                { 16, 64 },
                false,
                TEXTURE_ASSET_ID::DEFAULT
            )
        );
        tut_ext_3.connects.push_back(
            createConnect(
                LEVEL::TUT_INT_3,
                { 368, 32 },
                { 560, 288 },
                { 128, 16 },
                false,
                TEXTURE_ASSET_ID::DEFAULT
            )
        );
        
        tut_int_3.directory = "tutorial/tut_int_3";
        tut_int_3.startPos = { 368, 32 };
        tut_int_3.connects.push_back(
            createConnect(
                LEVEL::TUT_EXT_3,
                { 512, 192 },
                { 336, -16 },
                { 128, 16 },
                false,
                TEXTURE_ASSET_ID::DEFAULT
            )
        );

    }
};

extern LevelRegistry levels;