#ifndef GAMEELEMENT_H
#define GAMEELEMENT_H

#include "linkedlist.h"

//World
typedef struct WorldStruct {
    LinkedList* elements;
    int height;
    int width;
} World;

World* world_init(int, int);
void update(World*);

//Steering Controller:
typedef struct SteeringControllerStruct {
    LinkedList* behaviors;
} SteeringController;

SteeringController* init_steering_controller();

//Game Element
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

//Steering Behaviors:
void wander(GameElement* game_element);

#endif
