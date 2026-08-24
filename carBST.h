#ifndef CARBST_H
#define CARBST_H
#include <stdlib.h>
#include <stdio.h>
#include "car.h" // Include the Car type
#include "stationBST.h" // Include the StationBST type


typedef struct tCar_t {
    Car p2car;
    struct tCar_t* left;
    struct tCar_t* right;
} *tCar;

typedef struct CarBST_t {
    tCar root;
} *CarBST;

// Function declarations
CarBST create_car_bst();// Function to create a new Car Binary Search Tree

void insertCar(CarBST tree, Car car);// Function to insert a car into the binary search tree

Car findCar(CarBST tree, Car car);// Function to find a car in the binary search tree

Car findCarByLicense(CarBST tree, const char* license);// Function to find a car by its license in the binary search tree

void removeCar(CarBST tree, Car car);// Function to remove a car from the binary search tree

void inorderTraversal(CarBST tree);//Function to perform inorder traversal of the car binary search tree

void freeCarBST(CarBST tree);// Function to free the memory allocated for the car binary search tree

void loadCarsFromFile(CarBST carTree, StationBST stationTree, const char* filename);// Function to load cars from a file into the car binary search tree

void insertCarFromLine(CarBST carTree, StationBST stationTree, const char* line);// Function to insert a car from a line of text

void printCar(Car car);// Function to print car details

int getTreeHeight(tCar node);// Function to get the height of the car binary search tree

#endif // CARBST_H