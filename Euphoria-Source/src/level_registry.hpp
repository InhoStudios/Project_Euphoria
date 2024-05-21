#pragma once

#include "tiny_ecs_registry.hpp"
#include "common.hpp"
#include "world_init.hpp"

void loadLevel();

void loadGeometryFile(RenderSystem* renderer, std::string file_path);

class LevelRegistry
{
public:
    LevelRegistry() {}
    //
private:
    //
};

extern LevelRegistry levels;