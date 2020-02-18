#ifndef WORLD_H
#define WORLD_H

#include "linkedlist.h"

typedef struct WorldStruct {
    LinkedList* elements;
    int height;
    int width;
} World;

World* world_init(int, int);
void update(World*);

#endif
