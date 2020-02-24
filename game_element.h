#ifndef GAMEELEMENT_H
#define GAMEELEMENT_H

#include "linkedlist.h"
#include "vector.h"

//World
typedef struct WorldStruct {
    LinkedList* moving_elements;
    LinkedList* walls;
    int height;
    int width;
    int current_time;
} World;

World* world_init(int, int);
void world_update(World*, int);

struct SteeringControllerStruct;
struct GameElementStruct;

//Steering Controller:
typedef struct SteeringControllerStruct {
    LinkedList* behaviors;
    struct GameElementStruct* owner;
} SteeringController;

//Game Element
typedef struct GameElementStruct {
    void (*render)(struct GameElementStruct*);
    Vector position;
    Vector velocity;
    Vector heading;
    Vector heading_perp;
    float direction_radians;
    float direction_degrees;
    float mass;
    float max_force, max_speed;
    float height, width;
    float red, green, blue;
    SteeringController* steering_controller;
    void (*update)(struct GameElementStruct*);
} GameElement;

typedef struct WallStruct {
    Vector point1, point2;
} Wall;

Wall* init_wall(double, double, double, double);
void wall_render(Wall*);

SteeringController* init_steering_controller(GameElement*);
Vector calculate_force(SteeringController*);


GameElement* game_element_init(
    Vector,
    Vector,
    float,
    float,
    float, 
    float,
    float,
    float, 
    float, 
    float,
    void (*render)(GameElement*));
void game_element_update(GameElement*, int);
void game_element_update_heading(GameElement*);
void render1();

//Steering Behaviors:
Vector wander(GameElement* game_element);

#endif
