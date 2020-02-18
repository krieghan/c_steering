#include <stdlib.h>
#include "steering.h"
#include "steeringbehaviors.h"
#include "linkedlist.h"
#include "vector.h"

SteeringController* init_steering_controller(){
    SteeringController* steering_controller = malloc(
        sizeof(SteeringController));
    steering_controller->behaviors = init_linked_list();
    append_to_list(
        steering_controller->behaviors,
        &wander);
    return steering_controller;
}

