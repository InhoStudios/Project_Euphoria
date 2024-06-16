#include "level_registry.hpp"
#include <iostream>

enum class Tiles {
    SOLID = 0,
    INTERACTABLE = 1,

    ENEMY = 72,
};

void loadLevel(LEVEL l) {
    // clear level elements
    while (registry.levelElements.entities.size() > 0)
        registry.remove_all_components_of(registry.levelElements.entities.back());
    // load level data

    Level& level = levels.get(l);

    createBackground({
        {312, 176},
        {640, 368},
        0.4f,
        TEXTURE_ASSET_ID::BG_TUT_INT_1
    });

    // temp: load geometry file
    loadGeometryFile(level_path(level.directory + "/geometry.png"));
    loadEntityFile(level_path(level.directory + "/entities.png"));
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

            if (opacity == 0) continue;

            int xTo = xx * TILE_SIZE, yTo = yy * TILE_SIZE;

            // create different tiles
            if (opacity == 255) {
                switch (tile) {
                case Tiles::SOLID:
                {
                    Tiles l = Tiles::SOLID, 
                        ul = Tiles::SOLID, 
                        u = Tiles::SOLID, 
                        ur = Tiles::SOLID, 
                        r = Tiles::SOLID, 
                        dr = Tiles::SOLID, 
                        d = Tiles::SOLID, 
                        dl = Tiles::SOLID;
                    uint8_t o_l = 255,
                        o_ul = 255,
                        o_u = 255,
                        o_ur = 255,
                        o_r = 255,
                        o_dr = 255,
                        o_d = 255,
                        o_dl = 255;
                    int t_index = 8;
                    if (xx > 0) {
                        l = (Tiles)imageData[num_channels * ((yy) * im_width + (xx - 1))];
                        o_l = imageData[num_channels * ((yy)*im_width + (xx - 1)) + 1];
                        if (yy > 0) {
                            ul = (Tiles)imageData[num_channels * ((yy - 1) * im_width + (xx - 1))];
                            o_ul = imageData[num_channels * ((yy - 1) * im_width + (xx - 1)) + 1];
                        }
                        if (yy < im_height - 1) {
                            dl = (Tiles)imageData[num_channels * ((yy + 1) * im_width + (xx - 1))];
                            o_dl = imageData[num_channels * ((yy + 1) * im_width + (xx - 1)) + 1];
                        }
                    }
                    if (xx < im_width - 1) {
                        r = (Tiles)imageData[num_channels * ((yy)*im_width + (xx + 1))];
                        o_r = imageData[num_channels * ((yy)*im_width + (xx + 1)) + 1];
                        if (yy > 0) {
                            ur = (Tiles)imageData[num_channels * ((yy - 1) * im_width + (xx + 1))];
                            o_ur = imageData[num_channels * ((yy - 1) * im_width + (xx + 1)) + 1];
                        }
                        if (yy < im_height - 1) {
                            dr = (Tiles)imageData[num_channels * ((yy + 1) * im_width + (xx + 1))];
                            o_dr = imageData[num_channels * ((yy + 1) * im_width + (xx + 1)) + 1];
                        }
                    }
                    if (yy > 0) {
                        u = (Tiles)imageData[num_channels * ((yy - 1) * im_width + (xx))];
                        o_u = imageData[num_channels * ((yy - 1) * im_width + (xx)) + 1];
                    }
                    if (yy < im_height - 1) {
                        d = (Tiles)imageData[num_channels * ((yy + 1) * im_width + (xx))];
                        o_d = imageData[num_channels * ((yy + 1) * im_width + (xx)) + 1];
                    }
                    // selection logic: edges
                    if (u != Tiles::SOLID || o_u == 0) {
                        if (r != Tiles::SOLID || o_r == 0) {
                            if (d != Tiles::SOLID || o_d == 0) {
                                if (l != Tiles::SOLID || o_l == 0) {
                                    t_index = 19;
                                } else {
                                    t_index = 22;
                                }
                            } else if (l != Tiles::SOLID || o_l == 0) {
                                t_index = 36;
                            } else if (dl != Tiles::SOLID || o_dl == 0) {
                                t_index = 39;
                            } else {
                                t_index = 2;
                            }
                        } else if (l != Tiles::SOLID || o_l == 0) {
                            if (d != Tiles::SOLID || o_d == 0) {
                                t_index = 20;
                            } else if (dr != Tiles::SOLID || o_dr == 0) {
                                t_index = 40;
                            } else {
                                t_index = 0;
                            }
                        } else if (d != Tiles::SOLID || o_d == 0) {
                            t_index = 21;
                        }
                        // corners
                        else if (dl != Tiles::SOLID || o_dl == 0) {
                            if (dr != Tiles::SOLID || o_dr == 0) {
                                t_index = 28;
                            } else {
                                t_index = 27;
                            }
                        } else if (dr != Tiles::SOLID || o_dr == 0) {
                            t_index = 29;
                        } else {
                            t_index = 1;
                        }
                    } else if (r != Tiles::SOLID || o_r == 0) {
                        if (l != Tiles::SOLID || o_l == 0) {
                            if (d != Tiles::SOLID || o_d == 0) {
                                t_index = 38;
                            } else {
                                t_index = 37;
                            }
                        } else if (d != Tiles::SOLID || o_d == 0) {
                            if (ul != Tiles::SOLID || o_ul == 0) {
                                t_index = 41;
                            } else {
                                t_index = 4;
                            }
                        } else if (ul != Tiles::SOLID || o_ul == 0) {
                            if (dl != Tiles::SOLID || o_dl == 0) {
                                t_index = 31;
                            } else {
                                t_index = 32;
                            }
                        } else if (dl != Tiles::SOLID || o_dl == 0) {
                            t_index = 30;
                        } else {
                            t_index = 3;
                        }
                    } else if (d != Tiles::SOLID || o_d == 0) {
                        if (l != Tiles::SOLID || o_l == 0) {
                            if (ur != Tiles::SOLID || o_ur == 0) {
                                t_index = 42;
                            } else {
                                t_index = 6;
                            }
                        } else if (ul != Tiles::SOLID || o_ul == 0) {
                            if (ur != Tiles::SOLID || o_ur == 0) {
                                t_index = 34;
                            } else {
                                t_index = 33;
                            }
                        } else if (ur != Tiles::SOLID || o_ur == 0) {
                            t_index = 35;
                        } else {
                            t_index = 5;
                        }
                    } else if (l != Tiles::SOLID || o_l == 0) {
                        if (ur != Tiles::SOLID || o_ur == 0) {
                            if (dr != Tiles::SOLID || o_dr == 0) {
                                t_index = 25;
                            } else {
                                t_index = 24;
                            }
                        } else if (dr != Tiles::SOLID || o_dr == 0) {
                            t_index = 26;
                        } else {
                            t_index = 7;
                        }
                    } 
                    // only corners
                    else if (dr != Tiles::SOLID || o_dr == 0) {
                        if (dl != Tiles::SOLID || o_dl == 0) {
                            if (ul != Tiles::SOLID || o_ul == 0) {
                                if (ur != Tiles::SOLID || o_ur == 0) {
                                    t_index = 23;
                                } else {
                                    t_index = 43;
                                }
                            } else if (ur != Tiles::SOLID || o_ur == 0) {
                                t_index = 44;
                            } else {
                                t_index = 14;
                            }
                        } else if (ul != Tiles::SOLID || o_ul == 0) {
                            if (ur != Tiles::SOLID || o_ur == 0) {
                                t_index = 45;
                            } else {
                                t_index = 17;
                            }
                        } else if (ur != Tiles::SOLID || o_ur == 0) {
                            t_index = 15;
                        } else {
                            t_index = 9;
                        }
                    } else if (dl != Tiles::SOLID || o_dl == 0) {
                        if (ur != Tiles::SOLID || o_ur == 0) {
                            if (ul != Tiles::SOLID || o_ul == 0) {
                                t_index = 46;
                            } else {
                                t_index = 18;
                            }
                        } else if (ul != Tiles::SOLID || o_ul == 0) {
                            t_index = 13;
                        } else {
                            t_index = 10;
                        }
                    } else if (ul != Tiles::SOLID || o_ul == 0) {
                        if (ur != Tiles::SOLID || o_ur == 0) {
                            t_index = 16;
                        } else {
                            t_index = 11;
                        }
                    } else if (ur != Tiles::SOLID || o_ur == 0) {
                        t_index = 12;
                    }
                    // no vacant case 
                    else {
                        t_index = 8;
                    }
                    createTiledSolid({ xTo, yTo }, { TILE_SIZE, TILE_SIZE }, TEXTURE_ASSET_ID::SOLID_TILES, t_index);
                    break;
                }
                case Tiles::INTERACTABLE:
                    createItem({ xTo, yTo }, { TILE_SIZE, TILE_SIZE }, { 1.f, 1.f, }, true);
                    break;
                }
            }
        }
    }

    // camera bounds
    registry.gameManagers.components[0].bounds = { TILE_SIZE * im_width, TILE_SIZE * im_height };

    stbi_image_free(imageData);
}

void loadEntityFile(std::string file_path) {
    int im_width, im_height, num_channels;
    uint8_t* imageData = stbi_load(file_path.c_str(), &im_width, &im_height, &num_channels, 0);

    for (int yy = 0; yy < im_height; yy++) {
        for (int xx = 0; xx < im_width; xx++) {
            Tiles tile = (Tiles)imageData[num_channels * (yy * im_width + xx)];
            uint8_t opacity = imageData[num_channels * (yy * im_width + xx) + 1];
            int xTo = xx * TILE_SIZE, yTo = yy * TILE_SIZE;

            // create different tiles
            switch (tile) {
            case Tiles::ENEMY:
                createEnemy({ xTo, yTo });
                break;
            }
        }
    }

    stbi_image_free(imageData);
}

LevelRegistry levels;