#ifndef QCAR_H_
#define QCAR_H_
#include <stdbool.h>
#include "car.h"
#include "carNode.h"

/** This data type represents the Queue of Cars */

typedef struct qCar_t {
    CarNode front; // Pointer to the first node in the queue
    CarNode rear; // Pointer to the last node in the queue
} *qCar;


qCar qCarCreate(); // Function to create a new queue of cars

void qCarDestroy(qCar queue); // Function to destroy the queue and free memory  

bool qCarIsEmpty(qCar queue); // Function to check if the queue is empty

qCar qCarCopy(qCar queue); // Function to copy the queue, if needed

void qCarEnqueue(qCar queue, Car car); // Function to add a car to the queue

Car qCarDequeue(qCar queue); // Function to remove a car from the queue

void qCarPrintByLicense(qCar queue); // Function to print the queue of cars by license number

Car qCarFindByLicense(qCar queue, const char* license); // Function to find a car by its license in the queue

int qCarSize(qCar queue);// Function to get the size of the queue

Car qCarPeek(qCar queue);// Function to peek at the front car in the queue without removing it

#endif // QCAR_H_