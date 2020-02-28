#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdbool.h>

typedef struct LinkedListNodeStruct {
    void* data;
    struct LinkedListNodeStruct* next;
} LinkedListNode;

typedef struct LinkedListStruct {
    LinkedListNode* head;
    LinkedListNode* tail;
    int size;
} LinkedList;


void append_to_list(LinkedList*, void*);
LinkedList* init_linked_list();
void destruct_linked_list(LinkedList*, bool);

#endif

