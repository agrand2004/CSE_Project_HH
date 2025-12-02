#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>

typedef struct LinkedList_s
{
    int id;
    double sensorData;
    unsigned int timestamp;
    struct LinkedList_s *next;
} LinkedList_t;

// TODO: handle timestamp in the different functions
LinkedList_t *readSensor(int id);
LinkedList_t *findMaxLinkedList(LinkedList_t *linkedList);
LinkedList_t *findMinLinkedList(LinkedList_t *linkedList);

void insertFirst(LinkedList_t **first, LinkedList_t *el);
int isMember(LinkedList_t **first, LinkedList_t *el);
int removeElement(LinkedList_t **first, LinkedList_t *el);
void sortLinkedList(LinkedList_t **list);
void freeLinkedList(LinkedList_t *list);

#endif // LINKED_LIST_H