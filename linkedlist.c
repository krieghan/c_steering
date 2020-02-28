#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

void append_to_list(LinkedList* list, void* data){
    LinkedListNode* node = malloc(sizeof(LinkedListNode));
    node->data = data;
    node->next = NULL;
    if (!list->head){
        list->head = node;
        list->tail = node;
        return;
    }
    list->tail->next = node;
    list->tail = node;
    list->size += 1;
}

LinkedList* init_linked_list(){
    LinkedList* linked_list = malloc(sizeof(LinkedList));
    linked_list->head = NULL;
    linked_list->tail = NULL;
    linked_list->size = 0;
    return linked_list;
}

void destruct_linked_list(LinkedList* list, bool destroy_contents){
    LinkedListNode* current_element = list->head;
    LinkedListNode* next_element;
    while (current_element){
        next_element = current_element->next;
        if (destroy_contents){
            free(current_element->data);
        }
        free(current_element);
        current_element = next_element;
    }
    free(list);
}
