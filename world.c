#include <stdlib.h>
#include <stdio.h>
#include "game_element.h"
#include "linkedlist.h"

World* world_init(int height, int width){
    World* world = malloc(sizeof(World));
    world->moving_elements = init_linked_list();
    world->walls = init_linked_list();
    world->height = height;
    world->width = width;
    world->current_time = 0;
    return world;
}

void world_update(World* world, int current_time){
    GameElement* current_element;
    LinkedListNode* current_node;
    current_node = world->moving_elements->head;
    int time_elapsed = current_time - world->current_time;
    world->current_time = current_time;

    while(current_node){
        current_element = (GameElement*)current_node->data;
        game_element_update(current_element, time_elapsed);
        current_node = current_node->next;
    }
}
