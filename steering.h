#ifndef STEERINGCONTROLLER_H
#define STEERINGCONTROLLER_H
#include "linkedlist.h"

typedef struct SteeringControllerStruct {
    LinkedList* behaviors;
} SteeringController;

SteeringController* init_steering_controller();

#endif
