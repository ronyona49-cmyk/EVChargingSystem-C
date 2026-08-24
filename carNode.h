#ifndef CARNODE_H
#define CARNODE_H
#include "car.h"
#include <stdlib.h>

/** This data type is for the Queue of Cars */

typedef struct carNode_t {
    Car car;               // Pointer to the car
    struct carNode_t* next; // Pointer to the next car node in the linked list
} *CarNode;


CarNode carNodeCreate(Car car);// Function to create a new car node

void carNodeDestroy(CarNode node);// Function to destroy a car node and free memory

CarNode carNodeCopy(CarNode node);// Function to create a copy of a car node

Car getCarFromNode(CarNode node);// Function to get the car from a car node

CarNode getNextCarNode(CarNode node);// Function to get the next car node in the linked list

void setNextCarNode(CarNode node, CarNode next);// Function to set the next car node in the linked list

#endif // CARNODE_H