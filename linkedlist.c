#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

void append_to_list(LinkedList* list, void* data){
    LinkedListNode* node = malloc(sizeof(LinkedListNode));
    LinkedListNode* current_node;
    node->data = data;
    node->next = NULL;
    if (!list->head){
        list->head = node;
        return;
    }
    current_node = list->head;
    while (current_node->next){
        current_node = current_node->next;
    }
    current_node->next = node;
}

LinkedList* init_linked_list(){
    LinkedList* linked_list = malloc(sizeof(LinkedList));
    linked_list->head = NULL;
    return linked_list;
}
