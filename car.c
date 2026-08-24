#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include "car.h"


Car carCreate(const char license[PLATE_SIZE], PortType type, double total, Port pPort, int inqueue)// Creates a new car by choose
{
    Car car = (Car)malloc(sizeof(struct car_t));
	if (!car)// Handle memory allocation failure
    {
        printf("Memorry allocation failed.\n");
        return NULL;
    }
	// Initialize the car's fields
    strncpy(car->nLicense, license, PLATE_SIZE - 1);
    car->nLicense[PLATE_SIZE - 1] = '\0';
    car->portType = type;
    car->totalPayed = total;
    car->pPort = pPort;
    car->inqueue = inqueue;
    return car;
};


void carDestroy(Car car)// Destroys the car and frees memory
{
    if (car) {
        free(car);
    }
};

int compareCars(Car car1, Car car2)// Compares two cars by their license numbers
{
    if (car1 == NULL && car2 == NULL) return 0; // Both are NULL
    if (car1 == NULL) return -1; // car1 is less than car2
    if (car2 == NULL) return 1; // car1 is greater than car2
    return strcmp(car1->nLicense, car2->nLicense); // Compare by license number
};

Car copyCar(Car car)// Creates a copy of the car
{
    if (car == NULL) return NULL;// Handle null pointer
	Car newCar = carCreate(car->nLicense, car->portType, car->totalPayed, car->pPort, car->inqueue);// there is alloc check in carCreate
    return newCar;
};


const char* getCarLicense(Car car)// Returns the license number of the car
{
    if (car == NULL) return "";// Handle null pointer
    return car->nLicense;
};

PortType getCarPortType(Car car)// Returns the port type of the car
{
    if (car == NULL)   return FAST; // Handle null pointer, default to FAST
     return car->portType;
};

double getCarTotalPayed(Car car)// Returns the total amount paid by the car
{
    if (car == NULL) return 0.0; // Handle null pointer
       return car->totalPayed;
};

void setCarPort(Car car, Port port)// Sets the port for the car
{
    if (car == NULL) return; // Handle null pointer
     else car->pPort = port;
};

Port getCarPort(Car car)// Returns the port the car is charging at
{
    if (car == NULL) return NULL;// Handle null pointer
     return car->pPort;
};

int isCarInQueue(Car car)// Returns if car is in a queue (1 for yes, 0 for no)
{
    if (car == NULL) return INQUEUE_NO;// Handle null pointer
        return car->inqueue;
};

void setCarInQueue(Car car, int inqueue)// Sets car's inqueue status
{
    if (car == NULL)
    {
        printf("your car is empty"); // Handle null pointer
        return;
    }
    car->inqueue = inqueue;
};

