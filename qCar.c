#include "qCar.h"
#include <stdlib.h>




qCar qCarCreate()
{
    qCar queue = (qCar)malloc(sizeof(struct qCar_t));
	if (!queue)// Handle memory allocation failure
    {
        printf("Memorry allocation failed.\n");
        return NULL; 
    }
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}

void qCarDestroy(qCar queue)
{
	if (queue)// Check if the queue is not NULL
    {
        CarNode current = queue->front;
        while (current) {
            CarNode next = getNextCarNode(current); // Save next node
            carNodeDestroy(current); // Free the current node
            current = next; // Move to the next node
        }
        free(queue); // Free the queue structure itself
    }
}

qCar qCarCopy(qCar queue)
{
    if (!queue)  return NULL; // Handle null pointer
	// Create a new queue
    qCar newQueue = qCarCreate();
	if (!newQueue) return NULL; // Memory allocation failed,error message in qCarCreate

    CarNode current = queue->front;
	while (current) // Iterate through the original queue
    {
        Car car = getCarFromNode(current);
        if (car) {
            qCarEnqueue(newQueue, car); // Enqueue the car in the new queue
        }
        current = getNextCarNode(current); // Move to the next node
    }
    return newQueue;
}

bool qCarIsEmpty(qCar queue)// checks if the queue is empty
{
    return (queue == NULL || queue->front == NULL);
}

void qCarEnqueue(qCar queue, Car car) 
{
    if (queue == NULL || car == NULL) return; // Invalid queue or carNode
    

    CarNode carNode = carNodeCreate(car);
    if (carNode == NULL) {
		return; // Memory allocation failed,error message in carNodeCreate
    }
    
    if (qCarIsEmpty(queue)) {
		// Queue is empty, set both front and rear to the new node
        queue->front = carNode;
        queue->rear = carNode;
    }
    else {
        // Add to the end of the queue
        setNextCarNode(queue->rear, carNode);
        queue->rear = carNode;
    }
}

Car qCarDequeue(qCar queue) 
{
    if (qCarIsEmpty(queue)) return NULL; // Queue is empty
	// Get the car from the front node
    Car res = getCarFromNode(queue->front);
    if (res == NULL) {
		return NULL; // Failed to get car
    }
    CarNode dequeuedNode = queue->front; // Save the front node to free later
    queue->front = getNextCarNode(dequeuedNode); // Move front to the next node

    if (queue->front == NULL) {
        // If the queue is now empty, reset the rear pointer
        queue->rear = NULL;
    }

    carNodeDestroy(dequeuedNode); // Free the dequeued node
    return res; // Return the car from the dequeued node
}

void qCarPrintByLicense(qCar queue)
{
	if (qCarIsEmpty(queue)) return; // Queue is empty
    printf("Cars in the queue by license: ");
    CarNode current = queue->front;
	while (current)// Iterate through the queue
    {
        Car car = getCarFromNode(current);
        if (car) {
            printf("[Car License: %s] -> ", car->nLicense); // Print the license number
        }
        current = getNextCarNode(current); // Move to the next node
    }
    printf("[NULL]\n");
}


Car qCarFindByLicense(qCar queue, const char* license) 
{
    if(qCarIsEmpty(queue))  return NULL; // Queue is empty
    
    else if (license == NULL) {
        return NULL; // Invalid license input
    }
    CarNode current = queue->front;
	while (current)// Iterate through the queue
    {
        Car car = getCarFromNode(current);
        if (car && strcmp(car->nLicense, license) == 0) {
            return car; // Return the car if the license matches
        }
        current = getNextCarNode(current); // Move to the next node
    }
    return NULL; // Car not found
}

int qCarSize(qCar queue) 
{
	if (!queue) return 0;// Check for null pointer

    int count = 0;
    CarNode current = queue->front;
    while (current) {
        count++;
        current = getNextCarNode(current);
    }
    return count;
}

Car qCarPeek(qCar queue) {
	if (!queue || !queue->front)  return NULL;// Check for null pointer or empty queue
    return getCarFromNode(queue->front);
}

