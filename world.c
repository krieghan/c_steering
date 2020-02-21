#include <stdlib.h>
#include "game_element.h"
#include "linkedlist.h"

World* world_init(int height, int width){
    World* world = malloc(sizeof(World));
    world->elements = init_linked_list();
    world->height = height;
    world->width = width;
    return world;
}

void world_update(World* world, int timeElapsed){
    GameElement* current_element;
    LinkedListNode* current_node;
    current_node = world->elements->head;
    while(current_node){
        current_element = (GameElement*)current_node->data;
        game_element_update(current_element, timeElapsed);
        current_node = current_node->next;
    }
}
