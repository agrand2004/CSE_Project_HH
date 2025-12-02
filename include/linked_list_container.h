#ifndef LINKED_LIST_CONTAINER_H
#define LINKED_LIST_CONTAINER_H

#include "linked_list.h"

typedef struct LinkedListContainer_s
{
    LinkedList_t *head;
    LinkedList_t *tail;
} LinkedListContainer_t;

int insertLast(LinkedListContainer_t *container, LinkedList_t *node);
int popFirst(LinkedListContainer_t *container);

#endif // LINKED_LIST_CONTAINER_H
