#include "linked_list_container.h"

// Insert the node at the end of the linked list container
int insertLast(LinkedListContainer_t *container, LinkedList_t *node)
{
    LinkedList_t *lastNode = container->tail;
    if (lastNode != NULL)
        lastNode->next = node;
    else
        container->head = node; // If the list was empty, set head to the new node
    container->tail = node;
    node->next = NULL; // Ensure the new tail's next is NULL
    return 1; // Success
}

// Remove and free the first node from the linked list container
int popFirst(LinkedListContainer_t *container)
{
    if (container->head == NULL)
        return 0; // List is empty
    LinkedList_t *firstNode = container->head;
    container->head = firstNode->next;
    if (container->head == NULL)
        container->tail = NULL; // List is now empty
    free(firstNode);
    return 1; // Success
}
