#include <stdlib.h>
#include "world.h"
#include "game_element.h"
#include "linkedlist.h"

World* world_init(int height, int width){
    World* world = malloc(sizeof(World));
    world->elements = init_linked_list();
    world->height = height;
    world->width = width;
    return world;
}

void update(World* world){
    GameElement* current_element;
    LinkedListNode* current_node;
    current_node = world->elements->head;
    while(current_node){
        current_element = (GameElement*)current_node->data;
        //current_element->update(current_game_element);
        current_node = current_node->next;
    }
}
