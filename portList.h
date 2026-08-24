#ifndef PORTLIST_H
#define PORTLIST_H
#include <stdbool.h>
#include "car.h"
#include "date.h"
#include <stdlib.h>
#include "portList.h"
#include "date.h"
#include "Types.h"

typedef struct car_t* Car; // Forward declaration of Car type

typedef struct port_t {
    int num;               // port number
    PortType portType;     // FAST/MID/SLOW
    PortStatus status;     // occupied/free/out_of_order
    Car p2car;             // pointer to car being charged (can be NULL)
    Date tin;              // time charging started
    struct port_t* next;   // for linked list
} *Port;


typedef struct portList_t {
    Port head;
    Port current;
} *PortList;


PortList listCreate();// creates a new PortList

PortList listCopy(PortList list);// creates a copy of the PortList

void listDestroy(PortList list);// destroys the PortList and frees memory

Port listGetFirst(PortList list); // returns NULL if list is empty 

Port listFindPort(PortList list, Port port); // returns NULL if not found

Port listInsertFirst(PortList list, Port Port);// inserts a port at the beginning of the list

Port listInsertLast(PortList list, Port Port);// inserts a port at the end of the list

Port listRemovePort(PortList list, Port port);// removes a port from the list, returns the removed port or NULL if not found

Port listGetNext(PortList list, Port port); // returns NULL if not found

void listClear(PortList list);// clears the list, frees all ports in the list

int listGetSize(PortList list);// returns the number of ports in the list

bool listIsEmpty(PortList list);// returns true if the list is empty

void listPrint(PortList list); // prints the ports in the list

void listPrintIds(PortList list); // prints the IDs of the ports in the list

/** Helper functions to deal with Port */

Port createPort(int num, PortType type, PortStatus status, int year, int month, int day, int hour, int minute);// creates a new Port

void destroyPort(Port port);// destroys a Port and frees memory

const char* portTypeToString(PortType type);// converts PortType to string

bool assignCar2port(Car car, Port port);// assigns a car to a port

#endif // PORTLIST_H