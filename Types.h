#ifndef TYPES_H
#define TYPES_H


typedef enum { FALSE = 0, TRUE } Bool;// Boolean type for true/false values

typedef enum { FAST, MID, SLOW } PortType;// Port types for charging stations

typedef enum { OCCUPIED = 1, FREE, OUT_OF_ORDER } PortStatus;// Port status for charging stations

typedef enum { INVALID_INPUT = 1, STATION_NOT_FOUND, PORT_NOT_FOUND, CAR_NOT_FOUND, MEMORY_ALOC_FAILED, EMPTY_BST, FILE_OPEN_ERROR,EMPTY_INPUT,NO_PORTS,FAILED_CREATE_PORT,INPUT_MENU } ErrorCode;// Error codes for various operations

#endif // TYPES_H