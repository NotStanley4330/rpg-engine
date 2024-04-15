//
// Created by Michael Norris on 3/4/2024.
//

#include "maptiles.h"
#ifndef RPG_ENGINE_FILE_INPUT_OUTPUT_H
#define RPG_ENGINE_FILE_INPUT_OUTPUT_H

int ReadMapFile(char*, struct Bitmap*, struct MapTile**, int, int);

int WriteMapFile(char*, struct Bitmap*, struct MapTile**, int, int);

#endif //RPG_ENGINE_FILE_INPUT_OUTPUT_H
