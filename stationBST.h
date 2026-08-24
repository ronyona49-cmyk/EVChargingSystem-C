#ifndef STATIONBST_H_
#define STATIONBST_H_

#include "car.h" // for Car type
#include "portList.h" // for Port and PortList type
#include "qCar.h" // for qCar type
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct coordinate_t {
    double x;                // X coordinate
    double y;                // Y coordinate
} Coordinates;


typedef struct station_t {
    int id;                // Unique identifier for the station
    char* name;           // Name of the station
    int nPorts;          // Number of ports available at the station
    Coordinates coord; // Coordinates of the station
    PortList portsList;          // List of ports available at the station
    int nCars;           // Number of cars currently in queue at the station
    qCar carQueue;        // Queue of cars waiting to be charged
    struct station_t* left; // Pointer to left child in a binary tree
    struct station_t* right; // Pointer to right child in a binary tree
} *Station;


typedef struct StationBST_t {
    Station root; // Root of the binary search tree
}*StationBST;


StationBST create_station_bst(); // Function to create a new Station Binary Search Tree

void insertStation(StationBST tree, Station station); // Function to insert a station into the binary search tree

Station findStation(StationBST tree, Station station); // Function to search for a station in the binary search tree

void deleteStation(StationBST tree, Station station); // Function to delete a station from the binary search tree

void inorderTraversalStation(StationBST tree); // Function to perform inorder traversal of the station binary search tree

void freeStationBst(StationBST tree); // Function to free the memory allocated for the station binary search tree

Station searchStation(StationBST stationTree);

/** for Helper function searchStation in main */

Station searchStationByID(StationBST tree, int id); // Function to search for a station by its ID in the binary search tree

Station searchStationByName(StationBST tree, const char* name); // Function to search for a station by its name in the binary search tree

Station searchNearestStationByCoordinates(StationBST tree, double x, double y); // Function to search for a station by its coordinates in the binary search tree

Station searchStationByPortPointer(StationBST tree, Port port); // Function to search for a station by a port pointer in the binary search tree

Station searchStationByCarLicenseInQueue(StationBST tree, const char* license); // Function to search for a station by a car's license in the queue

Station stationCreate(int id, const char* name, int nPorts, double x, double y); // Function to create a new station ( a Node in the BST)

void stationDestroy(Station station); // Function to destroy a station and free its memory ( free a node in the BST)

Station stationCopy(Station station); // Function to create a copy of a station

int compareStations(Station station1, Station station2); // Function to compare two stations based on their IDs


void stationAddPort(Station station, Port port); // Function to add a port to a station's port list

void stationRemovePort(Station station, Port port); // Function to remove a port from a station's port list

void stationAddCarToQueue(Station station, Car car); // Function to add a car to the station's queue

void stationDequeueCarFromQueue(Station station); // Function to dequeue a car from the station's queue

int stationGetNumberOfCarsInQueue(Station station); // Function to get the number of cars in the station's queue

void stationPrint(Station station); // Function to print the details of a station for action #5

void locateNearestStation(StationBST stationTree); // Function to locate the nearest station based on user coordinates for action #1

void saveStationsToFile(StationBST tree, const char* filename); // Function to save stations to a file

void loadStationsFromFile(StationBST tree, const char* filename); // Function to load stations from a file

void insertStationFromLine(StationBST tree, const char* line); // Function to insert a station from a line in the file

void storeStationsToFile(StationBST tree, const char* filename); // Function to store stations to a file



#endif // STATIONBST_H_