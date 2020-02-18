#ifndef GAMEELEMENT_H
#define GAMEELEMENT_H
#include "steering.h" 

typedef struct GameElementStruct {
    void (*render)(struct GameElementStruct*);
    float xPosition, yPosition;
    float heading;
    float height, width;
    float red, green, blue;
    SteeringController* steering_controller;
} GameElement;

GameElement* game_element_init(
    float, 
    float,
    float,
    float,
    float, 
    float, 
    float, 
    float,
    void (*render)(GameElement*));
void render1();

#endif
