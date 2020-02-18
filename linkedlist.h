#ifndef LINKEDLIST_H
#define LINKEDLIST_H
typedef struct LinkedListNodeStruct {
    void* data;
    struct LinkedListNodeStruct* next;
} LinkedListNode;

typedef struct LinkedListStruct {
    LinkedListNode* head;
} LinkedList;


void append_to_list(LinkedList*, void*);
LinkedList* init_linked_list();

#endif

