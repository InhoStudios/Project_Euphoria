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
        tut_int_1.connects.push_back(
            createConnect(
                LEVEL::TUT_INT_2,
                { 24, 280 },
                { 624, 280 },
                { 16, 96 },
                false,
                TEXTURE_ASSET_ID::NO_SPRITE
            )
        );
        tut_int_1.connects.push_back(
            createConnect(
                LEVEL::TUT_INT_2,
                { 24, 184 },
                { 624, 184 },
                { 16, 96 },
                false,
                TEXTURE_ASSET_ID::NO_SPRITE
            )
        );
        tut_int_1.connects.push_back(
            createConnect(
                LEVEL::TUT_INT_2,
                { 24, 88 },
                { 624, 88 },
                { 16, 96 },
                false,
                TEXTURE_ASSET_ID::NO_SPRITE
            )
        );

        tut_int_2.directory = "tut_int_2";
        tut_int_2.startPos = { 32, 280 };
        tut_int_2.connects.push_back(
            createConnect(
                LEVEL::TUT_INT_1,
                { 600, 280 },
                { 0, 280 },
                { 16, 96 },
                false,
                TEXTURE_ASSET_ID::NO_SPRITE
            )
        );
        tut_int_2.connects.push_back(
            createConnect(
                LEVEL::TUT_INT_1,
                { 600, 184 },
                { 0, 184 },
                { 16, 96 },
                false,
                TEXTURE_ASSET_ID::NO_SPRITE
            )
        );
        tut_int_2.connects.push_back(
            createConnect(
                LEVEL::TUT_INT_1,
                { 600, 88 },
                { 0, 88 },
                { 16, 96 },
                false,
                TEXTURE_ASSET_ID::NO_SPRITE
            )
        );
        tut_int_2.connects.push_back(
            createConnect(
                LEVEL::TUT_EXT_1,
                { 32, 264 },
                { 1264, 232 },
                { 16, 96 },
                false,
                TEXTURE_ASSET_ID::NO_SPRITE
            )
        );

        tut_int_3.directory = "tut_int_3";
        tut_int_3.startPos = { 32, 264 };
        tut_int_3.connects.push_back(
            createConnect(
                LEVEL::TUT_EXT_1,
                { 600, 264 },
                { 0, 256 },
                { 16, 48 },
                false,
                TEXTURE_ASSET_ID::NO_SPRITE
            )
        );
        tut_int_3.connects.push_back(
            createConnect(
                LEVEL::TUT_EXT_2,
                { 600, 264 },
                { 0, 640 },
                { 16, 48 },
                false,
                TEXTURE_ASSET_ID::NO_SPRITE
            )
        );

        tut_int_4.directory = "tut_int_4";
        tut_int_4.startPos = { 24, 280 };
        tut_int_4.connects.push_back(
            createConnect(
                LEVEL::TUT_EXT_2,
                { 600, 424 },
                { 0, 264 },
                { 16, 64 },
                false,
                TEXTURE_ASSET_ID::NO_SPRITE
            )
        );

        tut_ext_1.directory = "tut_ext_1";
        tut_ext_1.startPos = { 32, 264 };
        tut_ext_1.connects.push_back(
            createConnect(
                LEVEL::TUT_INT_2,
                { 1248, 264 },
                { 0, 232 },
                { 16, 96 },
                false,
                TEXTURE_ASSET_ID::NO_SPRITE
            )
        );
        tut_ext_1.connects.push_back(
            createConnect(
                LEVEL::TUT_INT_3,
                { 24, 264 },
                { 624, 256 },
                { 16, 48 },
                false,
                TEXTURE_ASSET_ID::NO_SPRITE
            )
        );

        tut_ext_2.directory = "tut_ext_2";
        tut_ext_2.startPos = { 600, 264 };
        tut_ext_2.connects.push_back(
            createConnect(
                LEVEL::TUT_INT_3,
                { 24, 648 },
                { 624, 256 },
                { 16, 48 },
                false,
                TEXTURE_ASSET_ID::NO_SPRITE
            )
        );
        tut_ext_2.connects.push_back(
            createConnect(
                LEVEL::TUT_INT_4,
                { 24, 280 },
                { 624, 408 },
                { 16, 64 },
                false,
                TEXTURE_ASSET_ID::NO_SPRITE
            )
        );
    }
};

extern LevelRegistry levels;