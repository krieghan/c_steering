#include <stdlib.h>
#include "game_element.h"
#include "linkedlist.h"
#include "vector.h"

extern World* world;

SteeringController* init_steering_controller(GameElement* owner){
    SteeringController* steering_controller = malloc(
        sizeof(SteeringController));
    steering_controller->behaviors = init_linked_list();
    steering_controller->owner = owner;
    append_to_list(
        steering_controller->behaviors,
        &avoid_walls);
    append_to_list(
        steering_controller->behaviors,
        &flock);
    append_to_list(
        steering_controller->behaviors,
        &wander);
    return steering_controller;
}

Vector calculate_force(SteeringController* steering_controller){
    Vector force = {.x = 0, .y = 0};
    Vector force_for_behavior;
    double magnitude_for_behavior;
    double weight_for_behavior = 1;
    double remaining_reservoir = steering_controller->owner->max_force;
    LinkedListNode* current_node = steering_controller->behaviors->head;
    Vector (*behavior_function)(World*, GameElement*);
    while(current_node){
        behavior_function = (Vector (*)(World*, GameElement*))current_node->data;
        force_for_behavior = behavior_function(
            world,
            steering_controller->owner);
        force_for_behavior = vector_multiply_scalar(
                force_for_behavior, 
                weight_for_behavior);
        force_for_behavior = vector_truncate(
                force_for_behavior,
                remaining_reservoir);
        /*
        force_for_behavior = 
            vector_truncate(
                vector_multiply_scalar(
                    behavior_function(
                        steering_controller->owner),
                    weight_for_behavior),
                remaining_reservoir);
        */
        magnitude_for_behavior = vector_get_magnitude(force_for_behavior);
        remaining_reservoir -= magnitude_for_behavior;
        force = vector_add(force, force_for_behavior);
        current_node = current_node->next;
    }
    force = vector_truncate(force, steering_controller->owner->max_force);
    return force;
}
