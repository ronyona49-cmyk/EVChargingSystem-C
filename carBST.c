#include <stdio.h>
#include <stdlib.h>
#include "carBST.h"
#include <string.h>


void printCar(Car car); // Forward declaration for printing car details


tCar create_node(Car car)// Helper function to create a new node
{
    tCar new_node = (tCar)malloc(sizeof(struct tCar_t));
    if(!new_node)// Handle memory allocation failure
    {
		printf("Memorry allocation failed.\n");
        return NULL; 
	}
	// Initialize the new node
    new_node->p2car = car;
    new_node->left = new_node->right = NULL;
    return new_node;
}


tCar insert_node(tCar root, Car car)// Helper function to insert a value into the subtree
{
    if (root == NULL) return create_node(car);
    if (compareCars(car, root->p2car) < 0)
        root->left = insert_node(root->left, car);
    else if (compareCars(car, root->p2car) > 0)
        root->right = insert_node(root->right, car);
    return root;
}

Car search_node(tCar root, Car car)// Helper function to search for a value in the subtree
{
    if (root == NULL) return NULL;
    if (compareCars(car, root->p2car) == 0) return root->p2car;
    if (compareCars(car, root->p2car) < 0)
        return search_node(root->left, car);
    else
        return search_node(root->right, car);
}

void printCar(Car car) {
    if (car) {
        printf("License: %s, Port Type: %d, Total Paid: %.2f, In Queue: %d\n",
            car->nLicense, car->portType, car->totalPayed, car->inqueue);
    }
    else {
        printf("Car is NULL.\n");
    }
}

void inorder(tCar root)// Helper function for inorder traversal
{
    if (root) {
        inorder(root->left);
        printCar(root->p2car);
        inorder(root->right);
    }
}


tCar find_min(tCar root)// Helper function to find minimum value node
{
	if (root == NULL) return NULL; // Handle null pointer
    while (root->left) root = root->left;
    return root;
}


tCar delete_node_recursive(tCar root, Car car)// Helper function to delete a node
{
	if (root == NULL) return NULL; // Base case: not found
      if (compareCars(car, root->p2car) < 0)
        root->left = delete_node_recursive(root->left, car);
    else if (compareCars(car, root->p2car) > 0)
        root->right = delete_node_recursive(root->right, car);
    else {
        // CarBSTNode found
        if (!root->left) {
            tCar temp = root->right;
            free(root);
            return temp;
        }
        else if (!root->right) {
            tCar temp = root->left;
            free(root);
            return temp;
        }
        else {
            tCar min_node = find_min(root->right);
            root->p2car = min_node->p2car;
            root->right = delete_node_recursive(root->right, min_node->p2car);
        }
    }
    return root;
}


void free_nodes(tCar root)// Helper to free memory
{ 
    if (!root) return;
	// Recursively free left and right subtrees
    free_nodes(root->left);
    free_nodes(root->right);
    carDestroy(root->p2car); // Free the car associated with the node
    free(root);
}


CarBST create_car_bst()// CarBST public interface
{
    CarBST tree = (CarBST)malloc(sizeof(struct CarBST_t));
    if (!tree)// Handle memory allocation failure
    {
		printf("Memory allocation failed.\n");
		return NULL; 
    }
    tree->root = NULL;
    return tree;
}

void insertCar(CarBST tree, Car car)//inserts car into bst
{
    if (!tree) return; // Check for null tree
    tree->root = insert_node(tree->root, car);
}

Car findCar(CarBST tree, Car car) //finds car in the bst
{
	if (!tree || !car) return NULL;// Check for null pointers
    return search_node(tree->root, car);
}

Car findCarByLicenseRecursive(tCar root, const char* license) // Helper function to find a car by license recursively  
{
    if (root == NULL) return NULL; // Base case: not found  

    if (root->p2car == NULL) {
        printf("Error: root->p2car is NULL.\n");
        return NULL;
    }

    if (strcmp(root->p2car->nLicense, license) == 0) return root->p2car; // Found  

    if (strcmp(root->p2car->nLicense, license) > 0) {
        return findCarByLicenseRecursive(root->left, license); // Search in left subtree  
    }

    return findCarByLicenseRecursive(root->right, license); // Search in right subtree  
}


Car findCarByLicense(CarBST tree, const char* license) // Public interface to find a car by license
{
    if(!license || strlen(license) >= PLATE_SIZE) {
        return NULL; // Check for null or invalid license
	}
    return findCarByLicenseRecursive(tree->root, license); // Call the recursive function
}


void removeCar(CarBST tree, Car car)// Deletes a car from the BST
{
    if (!tree || !car)
    {
        return; // Check for null pointers
    }
    if (tree->root == NULL) return; // Tree is empty
    // Call the recursive delete function
    tree->root = delete_node_recursive(tree->root, car);
}



void inorderTraversal(CarBST tree) //performs inorder traversal of the BST
{
    if (!tree || !tree->root)  return; // Check for null tree or empty tree
    inorder(tree->root);
    printf("\n");
}

void freeCarBST(CarBST tree)//frees the memory allocated for the car BST 
{
    if (!tree) return; // Check for null tree
    free_nodes(tree->root);
    free(tree);
}


void loadCarsFromFile(CarBST tree, StationBST stationTree, const char* filename)//loading cars fromfile
{
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open \"%s\"\n", filename);
        return;
    }

    char line[256];
    // Skip the first line (header)
    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
		printf("The file is empty or only has a header.\n");
        return;
    }
    while (fgets(line, sizeof(line), file)) {
        insertCarFromLine(tree, stationTree, line);
    }
    fclose(file);
}


void insertCarFromLine(CarBST carTree, StationBST stationTree, const char* line)//inserts car from line
{
    if (!carTree || !stationTree || !line) {
        return; // Check for null pointers
	}
	// Define variables to hold parsed values
    char license[PLATE_SIZE];
    PortType portType;
    double totalPayed;
    int stationID;
    int portNumber;
    Bool inQueue;
    char portTypeStr[16];

    // Parse the line
    int parsed = sscanf(line, "%8[^,],%15[^,],%lf,%d,%d,%u", license, portTypeStr, &totalPayed, &stationID, &portNumber, (unsigned int*)&inQueue);
    if (parsed != 6) {
        printf("Invalid car line format: %s\n", line);
        return;
    }
    if (strcmp(portTypeStr, "FAST") == 0) {
        portType = FAST;
    }
    else if (strcmp(portTypeStr, "MID") == 0) {
        portType = MID;
    }
    else if (strcmp(portTypeStr, "SLOW") == 0) {
        portType = SLOW;
    }
    else {
        return; // Invalid port type    
    }
	// Create a new car from the parsed values
    Car newCar = carCreate(license, portType, totalPayed, NULL, inQueue);
    if (newCar == NULL) {
        printf("Failed to create car from line: %s\n", line);
        return;
    }
    insertCar(carTree, newCar);
 }

int getTreeHeight(tCar node)//get the height of the BST
{
    if (!node) return 0;
	// Recursively calculate the height of the left and right subtrees
        int left = getTreeHeight(node->left);
    int right = getTreeHeight(node->right);
    return 1 + (left > right ? left : right);
}



