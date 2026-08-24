#ifndef CAR_H_
#define CAR_H_

#include <stdbool.h>
#include <string.h>
#include "portList.h" 
#include "Types.h"

#define PLATE_SIZE 9
#define INQUEUE_NO 0
#define INQUEUE_YES 1

typedef struct port_t* Port; // Forward declaration of Port type

typedef struct car_t {
    char nLicense[PLATE_SIZE];  // unique license number
    PortType portType;    // type of charger the car uses
    double totalPayed;    // total amount paid
    Port pPort;          // pointer to the port it's charging at (can be NULL)
    int inqueue;         // whether it is currently in a queue
} *Car;

Car carCreate(const char license[PLATE_SIZE], PortType type, double total, Port pPort, int inqueue); // create new car

void carDestroy(Car car); // destroy car and free memory   

int compareCars(Car car1, Car car2); // Returns 0 if equal, -1 if car1 < car2, 1 if car1 > car2

Car copyCar(Car car); // Creates a copy of the car

const char* getCarLicense(Car car); // Returns the license number of the car

PortType getCarPortType(Car car); // Returns the port type of the car

double getCarTotalPayed(Car car); // Returns the total amount paid by the car

void setCarPort(Car car, Port port); // Sets the port for the car

Port getCarPort(Car car); // Returns the port the car is charging at

int isCarInQueue(Car car); // Returns if car is in a queue (1 for yes, 0 for no)

void setCarInQueue(Car car, int inqueue); // Sets cars in queue 


#endif // CAR_H_