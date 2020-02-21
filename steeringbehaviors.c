#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "game_element.h"
#include "vector.h"

#define WANDER_DISTANCE 30
#define WANDER_RADIUS 10
#define WANDER_JITTER .03


Vector wander(GameElement* game_element){
    Vector projectionVector = {.x = WANDER_DISTANCE, .y = 0};
    Vector centerToTargetVector = {.x = 0, .y = 0};
    Vector targetAdjustment = {
        .x = ((double)(rand())/RAND_MAX*2.0-1.0) * WANDER_JITTER,
        .y = ((double)(rand())/RAND_MAX*2.0-1.0) * WANDER_JITTER}; 
    centerToTargetVector = vector_set_magnitude(
        vector_add(
            centerToTargetVector,
            targetAdjustment),
        WANDER_RADIUS);
    Vector localTargetVector = vector_add(
            centerToTargetVector,
            projectionVector);
    Vector worldTargetVector = vector_to_world_space(
            localTargetVector,
            game_element->heading,
            game_element->heading_perp);
    //printf("%f %f\n", worldTargetVector.x, worldTargetVector.y);

    return worldTargetVector;
}

