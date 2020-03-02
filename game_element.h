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

typedef struct CanvasStruct {
    World* world;
    double screen_ratio, world_ratio;
    int viewport_left, viewport_bottom, viewport_top, viewport_right;
    int viewport_width, viewport_height;
} Canvas;

Canvas* canvas_init(World*);
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
    Line line;
} Wall;

Wall* init_wall(double, double, double, double);
void wall_render(Wall*);

SteeringController* init_steering_controller(GameElement*);
Vector calculate_force(SteeringController*, double);


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
LinkedList* game_element_get_neighbors(World*, GameElement*, double);
void render1();

//Steering Behaviors:
typedef struct SteeringBehaviorStruct {
    Vector (*behavior)(World*, GameElement*, double);
    double weight;
} SteeringBehavior;

SteeringBehavior* init_steering_behavior(
    Vector (*)(World*, GameElement*, double),
    weight);

Vector wander(World*, GameElement*, double);
Vector avoid_walls(World*, GameElement*, double);
Vector flock(World*, GameElement*, double);
#endif
