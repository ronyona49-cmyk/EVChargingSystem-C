#include "carNode.h"


CarNode carNodeCreate(Car car)//creates a new car node
{
    CarNode newNode = (CarNode)malloc(sizeof(struct carNode_t));
    if (!newNode)
    {
        printf("Memorry allocation failed.\n");
        return NULL; 
    }
    newNode->car = car;
    newNode->next = NULL; // Initialize next to NULL
    return newNode;
}

void carNodeDestroy(CarNode node)//destroys a car node and frees memory
{
    if (node)free(node);
}

CarNode carNodeCopy(CarNode node)//creates a copy of a car node
{
    if (node == NULL) return NULL; // Check for null pointer   
    CarNode newNode = carNodeCreate(node->car); // shallow copy of the car pointer
    newNode->next = node->next; // Shallow copy of the next pointer
    return newNode;
}

Car getCarFromNode(CarNode node)//returns the car from a car node
{
	if (node == NULL) return NULL; // Check for null pointer
    return node->car; // Return the car pointer
}

CarNode getNextCarNode(CarNode node)//returns the next car node in the linked list
{
	if (node == NULL) return NULL; // Check for null pointer
    return node->next; // Return the next car node pointer
}

void setNextCarNode(CarNode node, CarNode next)//sets the next car node in the linked list
{
    if (node != NULL) {
        node->next = next; // Set the next pointer to the provided node
    }
}

