#define _CRT_SECURE_NO_WARNINGS
#include "stationBST.h"
#include <math.h>
#include <float.h>

static Station insert_node_station(Station root, Station station);
static Station search_node_station(Station root, Station station);
static Station delete_node_recursive_station(Station root, Station station);
Station searchStationByNameRecursive(Station root, const char* name);
Station searchStationByPortPointerRecursive(Station root, Port port);
static void inorder(Station root);
void inorderSaveToFIle(Station root, FILE* file);
static void free_bst(Station root);
static Station find_min(Station root);
int getNumberOfOccupiedPorts(Station station);

// BST Functions
StationBST create_station_bst()//creates station BST
{
    StationBST tree = (StationBST)malloc(sizeof(struct StationBST_t));
	if (!tree)// Check for memory allocation failure
    {
        printf("Memorry allocation failed.\n");
        return NULL; 
    }
    tree->root = NULL; // Initialize the root to NULL
    return tree;
}

void insertStation(StationBST tree, Station station)//inserts station into the BST
{
    if (!tree) return; // Check for null pointer
    else if (!station) return; // Check for null pointer
    tree->root = insert_node_station(tree->root, station);
}

Station findStation(StationBST tree, Station station)//search station on BST
{
	if (!tree) return NULL;// Check for null pointer
    else if (!station) {
        return NULL;
    }
    return search_node_station(tree->root, station);
}

Station searchStationByID(StationBST tree, int id) //search station by id
{
	if (!tree) return NULL;// Check for null pointer
    Station temp = (Station)malloc(sizeof(struct station_t));
    if (!temp)
    {
		printf("Memory allocation failed.\n");
        return NULL; 
    }
	// Initialize the temporary station with the given ID
    temp->id = id;
    Station result = search_node_station(tree->root, temp);
    free(temp); // Free the temporary station
    return result; // Return the found station or NULL if not found
}

void deleteStation(StationBST tree, Station station)//delete station from bst
{
	if (!tree) return; //check for null pointer
    else if(!station)
    {
        return; // Check for null pointer
	}
    tree->root = delete_node_recursive_station(tree->root, station);
}

void inorderTraversalStation(StationBST tree)
{
    if (!tree) return;// Check for null pointer
    inorder(tree->root);
}

void freeStationBst(StationBST tree)//freeing the station BST
{
    if (!tree) return; // Check for null pointer
	free_bst(tree->root);// Free the nodes in the BST
    free(tree); // Free the tree structure itself
}

// Station Functions
Station stationCreate(int id, const char* name, int nPorts, double x, double y)//creates new station
{
    Station station = (Station)malloc(sizeof(struct station_t));
	if (!station)//handle memory allocation failure
    {
		printf("Memory allocation failed.\n");
        return NULL; 
    }
	// Initialize the station's fields
    station->id = id;
    station->name = _strdup(name); // Duplicate the name string
    station->nPorts = nPorts;
    station->coord.x = x;
    station->coord.y = y;
    station->portsList = listCreate(); // Initialize the ports list
    station->nCars = 0; // Initialize number of cars in queue
    station->carQueue = qCarCreate(); // Initialize the car queue
    station->left = NULL; // Initialize left child to NULL
    station->right = NULL; // Initialize right child to NULL

    return station;
}

void stationDestroy(Station station) //destroys a station and frees memory
{
    if (station) {
        free(station->name); // Free the name string
        listDestroy(station->portsList); // Destroy the ports list
        qCarDestroy(station->carQueue); // Destroy the car queue
        free(station); // Free the station structure
    }
}

Station stationCopy(Station station)//creates a copy of a station
{
    if (!station)     return NULL; // Handle null pointer
	// Create a new station with the same properties
    Station newStation = stationCreate(station->id, station->name, station->nPorts, station->coord.x, station->coord.y);
	if (!newStation) return NULL; // Memory allocation failed. error message in stationCreate

    newStation->portsList = listCopy(station->portsList); // Copy the ports list
    newStation->carQueue = qCarCopy(station->carQueue); // Copy the car queue
    newStation->nCars = station->nCars; // Copy the number of cars in queue
    newStation->left = station->left; // shallow copy left child
    newStation->right = station->right; // shallow copy right child

    return newStation;
}

int compareStations(Station station1, Station station2) // Compares two stations by their IDs
{
    if (!station1 && !station2) return 0; // Both are NULL
    if (!station1) return -1; // station1 is less than station2
    if (!station2) return 1; // station1 is greater than station2
    return station1->id - station2->id; // Compare by ID
}

void stationAddPort(Station station, Port port) {
    if (station && port) {
        listInsertLast(station->portsList, port); // Add the port to the ports list
    }
}

void stationRemovePort(Station station, Port port) // Removes a port from the station's ports list
{
    if (station && port) {
        listRemovePort(station->portsList, port); // Remove the port from the ports list
    }
}

void stationAddCarToQueue(Station station, Car car)// Adds a car to the station's queue
{
    if (station && car) {
        qCarEnqueue(station->carQueue, car); // Add the car to the queue
        station->nCars++; // Increment the number of cars in queue
    }
}

void stationDequeueCarFromQueue(Station station)// Dequeues a car from the station's queue
{
    if (station && station->carQueue) {
        qCarDequeue(station->carQueue); // Remove the car from the queue
        station->nCars--; // Decrement the number of cars in queue
    }
}

int stationGetNumberOfCarsInQueue(Station station)// Returns the number of cars in the station's queue
{
    if (station) {
        return station->nCars; // Return the number of cars in queue
    }
    return -1; // Handle null pointer
}

void stationPrint(Station station)// Prints the details of a station
{
    if (station) {
        printf("Station ID: %d\n", station->id);
        printf("Name: %s\n", station->name);
        printf("Number of Ports: %d\n", station->nPorts);
        printf("Coordinates: (%.2f, %.2f)\n", station->coord.x, station->coord.y);
        listPrintIds(station->portsList); // Print the IDs of the ports in the list
        printf("Number of Cars in Queue: %d\n", station->nCars);
        qCarPrintByLicense(station->carQueue); // Print the cars in the queue by license
        printf("Occupied Ports: %d\n", getNumberOfOccupiedPorts(station)); // Print the number of occupied ports
        printf("------------------------------------\n");
    }
}

// Helper functions for the binary search tree

Station insert_node_station(Station root, Station station)// Inserts a station into the binary search tree
{
    if (root == NULL) {
        Station newStation = stationCopy(station);
        newStation->left = NULL;
        newStation->right = NULL;
        return newStation;
    }
    if (compareStations(station, root) < 0) {
        root->left = insert_node_station(root->left, station);
    }
    else if (compareStations(station, root) > 0) {
        root->right = insert_node_station(root->right, station);
    }
    return root;
}

Station search_node_station(Station root, Station station)// Searches for a station in the binary search tree
{
    if (root == NULL) return NULL; // Not found
    if (compareStations(station, root) == 0) return root; // Found
	if (compareStations(station, root) < 0) // Compare the station IDs
    {
        return search_node_station(root->left, station);
    }
    else {
        return search_node_station(root->right, station);
    }
}

Station find_min(Station root) // Helper function to find the minimum value node in the subtree
{
    while (root && root->left) {
        root = root->left; // Traverse to the leftmost node
    }
    return root; // Return the minimum node
}


Station delete_node_recursive_station(Station root, Station station)// Deletes a station from the binary search tree
{
	if (root == NULL) return NULL; // Base case: not found
    if (compareStations(station, root) < 0) {
        root->left = delete_node_recursive_station(root->left, station);
    }
    else if (compareStations(station, root) > 0) {
        root->right = delete_node_recursive_station(root->right, station);
    }
    else {
        // Station found
        if (!root->left) {
            Station temp = root->right;
            stationDestroy(root); // Free the current node
            return temp;
        }
        else if (!root->right) {
            Station temp = root->left;
            stationDestroy(root); // Free the current node
            return temp;
        }
        else {
            // Node with two children: get the inorder successor
            Station minNode = find_min(root->right);
            root->id = minNode->id; // Copy the ID
            free(root->name); // Free the old name
            root->name = _strdup(minNode->name); // Copy the name
            root->coord = minNode->coord; // Copy coordinates
            root->portsList = listCopy(minNode->portsList); // Copy ports list
            root->carQueue = qCarCopy(minNode->carQueue); // Copy car queue
            root->nCars = minNode->nCars; // Copy number of cars in queue
            root->right = delete_node_recursive_station(root->right, minNode);
        }
    }
    return root;
}

Station find_min_station(Station root)// Helper function to find the minimum station in the subtree
{
    while (root && root->left) {
        root = root->left; // Traverse to the leftmost node
    }
    return root; // Return the minimum node
}


void free_bst(Station root)// Helper function to free the nodes in the tree
{
    if (root) {
        free_bst(root->left); // Free left subtree
        free_bst(root->right); // Free right subtree
        stationDestroy(root); // Free the current node
    }
}


void inorder(Station root)// Helper function to perform inorder traversal
{
	if (!root) return; // Base case: if the root is NULL, return
    else if (root) {
        inorder(root->left); // Traverse left subtree
        stationPrint(root); // Print the current station
        inorder(root->right); // Traverse right subtree
    }
}

void inorderSaveToFile(Station root, FILE* file) // Helper function to save stations to file in inorder traversal
{
    if (root) {
        inorderSaveToFile(root->left, file); // Traverse left subtree
        fprintf(file, "%d,%s,%d,%.2f,%.2f\n", root->id, root->name, root->nPorts, root->coord.x, root->coord.y);
        inorderSaveToFile(root->right, file); // Traverse right subtree
    }
}


void saveStationsToFile(StationBST tree, const char* filename)// Saves the stations in the BST to a file
{
	if (!tree || !filename) return; // Check for null pointers
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Failed to open %s for writing\n", filename);
        return;
    }
    fprintf(file, "ID,Name,NumberOfPorts,CoordX,CoordY\n"); // Write header

    inorderSaveToFile(tree->root, file); // Traverse the tree and save each station
    fclose(file); // Close the file
    printf("Stations saved to %s\n", filename);
}

void loadStationsFromFile(StationBST tree, const char* filename)// Loads stations from a file into the BST
{
    if (!tree || !filename) return; // Check for null pointers

    FILE* file = fopen(filename, "r");
    if (!file) {
		printf("Failed to open the file: %s\n", filename);
        return;
    }

    char line[256];
    // Skip the first line (header)
    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        return;
    }

    // Parse from the second line onwards
    while (fgets(line, sizeof(line), file)) {
        // printf("Processing line: %s", line);
        insertStationFromLine(tree, line); // line format: ID,StationName,NumOfPorts,CoordX,CoordY
    }
    fclose(file);
}

void insertStationFromLine(StationBST tree, const char* line)// Inserts a station into the BST from a line in the file
{
    if (!tree || !line)  return; // Check for null pointers
    int id, nPorts;
    double x, y;
    char name[100];
    // start scanning from the secoind line

    // Parse the line (assuming CSV format)
    sscanf(line, "%d,%99[^,],%d,%lf,%lf", &id, name, &nPorts, &x, &y);

    // printf("Parsed: ID=%d, Name=%s, nPorts=%d, Coordinates=(%.2f, %.2f)\n", id, name, nPorts, x, y);
    Station station = stationCreate(id, name, nPorts, x, y);
    if (station) {
        insertStation(tree, station); // Insert the station into the BST, it copies the station
        stationDestroy(station); // Free the temporary station
    }
}


int listGetNumberOfFunctioningPorts(PortList portsList)// Returns the number of functioning ports in the port list
{
    if (!portsList) return 0; // Handle null pointer
    int count = 0;
    Port current = portsList->head; // Assuming portsList has a head pointer
    while (current) {
        if (current->status != OUT_OF_ORDER) { // Assuming Port has a status field
            count++;
        }
        current = current->next; // Move to the next port
    }
    return count; // Return the count of functioning ports
}

int getNumberOfFunctioningPorts(Station station) // Returns the number of functioning ports in the station
{
    if (!station) return 0; // Handle null pointer
    if (!station->portsList) return 0; // Handle null pointer for ports list
    return listGetNumberOfFunctioningPorts(station->portsList); // Get the number of functioning ports from the port list
}


int listGetNumberOfFastPorts(PortList portsList)// Returns the number of fast ports in the port list
{
    if (!portsList) return 0; // Handle null pointer
    int count = 0;
    Port current = portsList->head; // Assuming portsList has a head pointer
    while (current) {
        if (current->portType == FAST) { // Assuming Port has a portType field
            count++;
        }
        current = current->next; // Move to the next port
    }
    return count; // Return the count of fast ports
}

int getNumberOfFastPorts(Station station) // Returns the number of fast ports in the station
{
    if (!station)  return 0; // Handle null pointer
    if (!station->portsList) return 0; // Handle null pointer for ports list
    return listGetNumberOfFastPorts(station->portsList); // Get the number of fast ports from the port list
}

int listGetNumberOfMidPorts(PortList portsList) // Returns the number of mid ports in the port list
{ 
    if (!portsList) return 0; // Handle null pointer
    int count = 0;
    Port current = portsList->head; // Assuming portsList has a head pointer
    while (current) {
        if (current->portType == MID) { // Assuming Port has a portType field
            count++;
        }
        current = current->next; // Move to the next port
    }
    return count; // Return the count of mid ports
}

int getNumberOfMidPorts(Station station) // Returns the number of mid ports in the station
{
    if (!station) return 0; // Handle null pointer
    if (!station->portsList) return 0; // Handle null pointer for ports list
    return listGetNumberOfMidPorts(station->portsList); // Get the number of mid ports from the port list
}


int listGetNumberOfSLOWPorts(PortList portsList) // Returns the number of slow ports in the port list
{ 
    if (!portsList) return 0; // Handle null pointer
    int count = 0;
    Port current = portsList->head; // Assuming portsList has a head pointer
    while (current) {
        if (current->portType == SLOW) { // Assuming Port has a portType field
            count++;
        }
        current = current->next; // Move to the next port
    }
    return count; // Return the count of slow ports
}

int getNumberOfSLOWPorts(Station station) // Returns the number of slow ports in the station
{
    if (!station) return 0; // Handle null pointer
    if (!station->portsList) return 0; // Handle null pointer for ports list
    return listGetNumberOfSLOWPorts(station->portsList); // Get the number of slow ports from the port list
}

int listGetNumberOfOccupiedPorts(PortList portsList)// Returns the number of occupied ports in the port list
{ 
    if (!portsList) return 0; // Handle null pointer
    int count = 0;
    Port current = portsList->head; // Assuming portsList has a head pointer
    while (current) {
        // printf("current port status: %u\n", current->status);
        if (current->status == OCCUPIED) { // Assuming Port has a status field
            count++;
        }
        current = current->next; // Move to the next port
    }
    return count; // Return the count of occupied ports
}

int getNumberOfOccupiedPorts(Station station)// Returns the number of occupied ports in the station
{
    if (!station) return 0; // Handle null pointer
    if (!station->portsList) return 0; // Handle null pointer for ports list
    return listGetNumberOfOccupiedPorts(station->portsList); // Get the number of occupied ports from the port list
}


void printNearestStation(Station station, double distance) // Prints the details of the nearest station
{
    if (station) {
        printf("Nearest Station:\n");
        printf("Station name: %s\n", station->name);
        printf("Station ID: %d\n", station->id);
        printf("Aerial distance: %.2f\n", distance);
        printf("Functioning ports: %d\n", getNumberOfFunctioningPorts(station));
        printf("Fast ports: %d\n", getNumberOfFastPorts(station));
        printf("Mid ports: %d\n", getNumberOfMidPorts(station));
        printf("Slow ports: %d\n", getNumberOfSLOWPorts(station));
        printf("Occupied ports: %d\n", getNumberOfOccupiedPorts(station));
        printf("Cars in queue: %d\n", station->nCars);
    }
    else {
        printf("No nearest station found.\n");
    }
}

double calculateDistance(double x1, double y1, double x2, double y2)// Helper function to calc distance between two coordinates
{
    printf("Calculating distance between (%.2f, %.2f) and (%.2f, %.2f)\n", x1, y1, x2, y2);
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

// Recursive helper to search the BST
void findAndPrintNearestStationRecursive(Station root, double userX, double userY, Station* nearest, double* minDistance) {
	if (!root) return; // Base case: if the root is NULL, return

    //printf("Visiting station: %d\n", root->id);
    //printf("Current minDistance: %.2f\n", *minDistance);
    double dist = calculateDistance(userX, userY, root->coord.x, root->coord.y);
    //printf("Checking station: %s at distance: %.2f\n", root->name, dist);
    if (dist < *minDistance) {
        *minDistance = dist;
        *nearest = root;
       // printf("New nearest station: %s at distance: %.2f\n", (*nearest)->name, *minDistance);
    }

    findAndPrintNearestStationRecursive(root->left, userX, userY, nearest, minDistance);
    findAndPrintNearestStationRecursive(root->right, userX, userY, nearest, minDistance);
}


void locateNearestStation(StationBST stationTree) {
    double userX, userY;
    printf("Enter your coordinates (x y): ");
    if (scanf("%lf %lf", &userX, &userY) != 2) {
        printf("Invalid input.\n");
        while (getchar() != '\n'); // flush input buffer
        return;
    }

    Station nearest = NULL;
    double minDistance = DBL_MAX;

    findAndPrintNearestStationRecursive(stationTree->root, userX, userY, &nearest, &minDistance);

    if (nearest) {
        printf("Nearest station found:\n");
        printf("Station ID: %d\n", nearest->id);
        printNearestStation(nearest, minDistance); // Print the nearest station details

    }
    else {
        printf("No stations found.\n");
    }
}

Station searchStationByNameRecursive(Station root, const char* name)// Recursive helper function to search for a station by name
{
	if (!root) return NULL; // Base case: if the root is NULL, return NULL
    printf("Visiting station: %s\n", root->name);
    if (strcmp(name, root->name) == 0) {
        return root;
    }

    // Search in the left subtree
    Station found = searchStationByNameRecursive(root->left, name);
    if (found) return found;

    // Search in the right subtree
    return searchStationByNameRecursive(root->right, name);
}

Station searchStationByName(StationBST tree, const char* name) {
	if (!tree) return NULL;// Check for null pointer
    else if (!name) return NULL;
    printf("Searching for station by name: %s\n", name);
    return searchStationByNameRecursive(tree->root, name);
}

static void findNearestHelper(Station node, double x, double y, Station* nearest, double* minDistance)// Recursive helper function to find the nearest station by coordinates
{
    if (!node)  return; // Base case: if node is NULL
    double distance = calculateDistance(x, y, node->coord.x, node->coord.y);
    if (distance < *minDistance) {
        *minDistance = distance;
        *nearest = node; // Update nearest station
    }

    // Traverse left and right subtrees
    findNearestHelper(node->left, x, y, nearest, minDistance);
    findNearestHelper(node->right, x, y, nearest, minDistance);
}

Station searchNearestStationByCoordinates(StationBST tree, double x, double y)// Searches for the nearest station by coordinates 
{
	if (!tree || !tree->root) return NULL; // Check for null pointer or empty tree
	// Initialize variables to keep track of the nearest station and minimum distance
    Station nearest = NULL;
    double minDistance = DBL_MAX;

    findNearestHelper(tree->root, x, y, &nearest, &minDistance); // Start the search from the root
    return nearest; // Return the nearest station found
}


Station searchStationByPortPointerRecursive(Station root, Port port)// Recursive helper function to search for a station by port pointer 
{
    if (!root || !port || !root->portsList)
    {
        return NULL; // Check for null pointers
    }
    if (listFindPort(root->portsList, port)) {
        return root; // Return the station if the port is found
    }
    Station found = searchStationByPortPointerRecursive(root->left, port);
    if (found) {
        return found; // Return the found station from the left subtree
    }

    return searchStationByPortPointerRecursive(root->right, port); // Search in the right subtree
}


Station searchStationByPortPointer(StationBST tree, Port port)// Searches for a station by port pointer in the binary search tree
{
    if (!tree || !port)
    {
        return NULL; // Check for null pointers
    }
    return searchStationByPortPointerRecursive(tree->root, port);
}


Station searchStationByCarLicenseInQueueRecursive(Station root, const char* license)// Recursive helper function to search for a station by a car's license in the queue
{
    if (!root || !license)
    {
        return NULL; // Check for null pointers
    }
    if (qCarFindByLicense(root->carQueue, license)) {
        return root; // Return the station if the car is found in the queue
    }

    // Search in the left subtree
    Station found = searchStationByCarLicenseInQueueRecursive(root->left, license);
    if (found) return found; // Return the found station from the left subtree

    // Search in the right subtree
    return searchStationByCarLicenseInQueueRecursive(root->right, license);
}


Station searchStationByCarLicenseInQueue(StationBST tree, const char* license)// Searches for a station by a car's license in the queue
{
    if (!tree || !license)
    {
        return NULL; // Check for null pointers
    }
    return searchStationByCarLicenseInQueueRecursive(tree->root, license);
}