#include "linked_list.h"

// Create a new node with sensor data
LinkedList_t *readSensor(int id)
{
    LinkedList_t *list = malloc(sizeof(LinkedList_t));
    if (list == NULL) {
        return NULL; // Handle memory allocation failure
    }
    list->id = id;
    list->sensorData = (double)(rand() % 100) / 100.0;
    list->next = NULL;
    return list;
}

// Find the node with maximum sensorData value
LinkedList_t *findMaxLinkedList(LinkedList_t *linkedList)
{
    if (linkedList == NULL)
        return NULL;
    double valueMax = linkedList->sensorData;
    LinkedList_t *current = linkedList;
    LinkedList_t *returnedValue = linkedList; // Initialize to first node

    while (current != NULL)
    {
        if (current->sensorData > valueMax)
        {
            valueMax = current->sensorData;
            returnedValue = current;
        }
        current = current->next;
    }
    return returnedValue;
}

// Find the node with minimum sensorData value
LinkedList_t *findMinLinkedList(LinkedList_t *linkedList)
{
    if (linkedList == NULL)
        return NULL;
    double valueMin = linkedList->sensorData;
    LinkedList_t *current = linkedList;
    LinkedList_t *returnedValue = linkedList; // Initialize to first node

    while (current != NULL)
    {
        if (current->sensorData < valueMin)
        {
            valueMin = current->sensorData;
            returnedValue = current;
        }
        current = current->next;
    }
    return returnedValue;
}

// Insert element at beginning of lisst and update first pointer
void insertFirst(LinkedList_t **first, LinkedList_t *el)
{
    if (el == NULL)
        return;
    el->next = *first;
    *first = el;
}

// Check if element is member of the list
int isMember(LinkedList_t **first, LinkedList_t *el)
{
    if (el == NULL || *first == NULL)
        return 0;

    LinkedList_t *current = *first;
    while (current != NULL)
    {
        if (current == el)
        {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

// Remove element from list without deleting it from memory
int removeElement(LinkedList_t **first, LinkedList_t *el)
{
    if (first == NULL || *first == NULL || el == NULL)
        return 0;

    // Removing first element
    if (*first == el)
    {
        *first = (*first)->next;
        el->next = NULL;
        return 1;
    }

    // Find element and its predecessor
    LinkedList_t *current = *first;
    LinkedList_t *prev = NULL;

    while (current != NULL && current != el)
    {
        prev = current;
        current = current->next;
    }

    // Remove element if found
    if (current == el)
    {
        prev->next = current->next;
        el->next = NULL;
        return 1;
    }

    return 0;
}

// Sort the linked list in descending order by sensorData
void sortLinkedList(LinkedList_t **list)
{
    if (*list == NULL || (*list)->next == NULL)
        return;

    LinkedList_t *temp = *list;
    LinkedList_t *sortedList = NULL;
    LinkedList_t *current = temp;

    while (current != NULL)
    {
        LinkedList_t *greatest = findMaxLinkedList(current);
        removeElement(&current, greatest);
        greatest->next = NULL;
        insertFirst(&sortedList, greatest);
    }
    *list = sortedList;
}

// Free all nodes in the linked list
void freeLinkedList(LinkedList_t *list)
{
    LinkedList_t *current = list;
    LinkedList_t *temp;

    while (current) {
        temp = current;
        current = current->next;
        free(temp);
    }
}
