#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "carBST.h"
#include "stationBST.h"
#include "portList.h"
#include "qCar.h"
#include "date.h"
#include<time.h>

#define NAME_SIZE 256

// Forward declarations
void updateFiles(StationBST stationTree, CarBST carTree); // Function to update files with current data
void displayError(ErrorCode code); // Function to display error messages



// Function for printing the menu
// This function displays the available actions to the user
void print_menu() {
    printf("=============================================\n");
    printf("  Welcome, please select an action number:   \n");
    printf("=============================================\n");
    printf(" 1. locNearSt         - Locate nearest station\n");
    printf(" 2. chargeCar         - Charge car\n");
    printf(" 3. checkCarStatus    - Check car status\n");
    printf(" 4. stopCharge        - Stop charge\n");
    printf(" 5. dispAllSt         - Display all stations\n");
    printf(" 6. dispCarsAtSt      - Display cars at station\n");
    printf(" 7. reportStStat      - Report of station's statistics\n");
    printf(" 8. dispTopCostumers  - Display top costumers\n");
    printf(" 9. addNewPort        - Add new port\n");
    printf("10. releasePorts      - Release charging ports\n");
    printf("11. remOutOrderPort   - Remove out of order ports\n");
    printf("12. remCustomer       - Remove customer\n");
    printf("13. closeSt           - Close station\n");
    printf("0. exitSys           - Exit system\n");
    printf("=============================================\n");
}


// Menu Functions - TBD

void locNearSt(StationBST stationTree) {
    locateNearestStation(stationTree); // Call the function to locate the nearest station
}

void chargeCar(StationBST stationTree, CarBST carTree) {
    char license[PLATE_SIZE];
    int stationID;

    // Ask for car license
    printf("Enter car license (8 characters): ");
    if (scanf("%8s", license) != 1) {
		displayError(INVALID_INPUT);
        while (getchar() != '\n'); // Clear input buffer
        return;
    }
    while (getchar() != '\n'); // Clear input buffer

    // Search for car in carTree
    Car car = findCarByLicense(carTree, license);
    if (!car) {
        printf("Car with license %s not found. Please provide details to add the car.\n", license);
    }

    // If car not found, prompt for details and add to system
    while (!car) {
        char portTypeStr[16];
        PortType portType;
        double totalPayed = 0.0;
        int valid = 1;

        // Port type
        do {
            printf("Enter port type (FAST/MID/SLOW): ");
            if (scanf("%15s", portTypeStr) != 1) {
                displayError(INVALID_INPUT);
                while (getchar() != '\n'); // Clear input buffer
                return;
            }
            if (strcmp(portTypeStr, "FAST") == 0)
                portType = FAST;
            else if (strcmp(portTypeStr, "MID") == 0)
                portType = MID;
            else if (strcmp(portTypeStr, "SLOW") == 0)
                portType = SLOW;
            else {
                printf("Invalid port type.");
                valid = 0;
            }
        } while (!valid && (valid = 1));

        while (getchar() != '\n'); // Clear input buffer

        // Total paid
        printf("Enter total amount paid (0.0 if not applicable): ");
        while (scanf("%lf", &totalPayed) != 1 || totalPayed < 0) {
            displayError(INVALID_INPUT);
            while (getchar() != '\n'); // Clear input buffer
        }

        while (getchar() != '\n'); // Clear input buffer

        // Create and insert car
        car = carCreate(license, portType, totalPayed, NULL, 0);
        if (!car) {
            printf("Failed to create car. Please re-enter details.\n");
            continue;
        }
        insertCar(carTree, car);
		updateFiles(stationTree, carTree); // Save the new car to file
    }

    // Ask for station ID
    printf("Enter station ID: ");
    while (scanf("%d", &stationID) != 1) {
		displayError(INVALID_INPUT);
        while (getchar() != '\n');
    }
    while (getchar() != '\n');

    // Find the station
    Station station = searchStationByID(stationTree, stationID);
    if (!station) {
        printf("Station with ID %d not found.\n", stationID);
    
        return;
    }

    // Try to find a compatible port
    Port compatiblePortAvailable = NULL;
    bool hasCompatiblePort = false;
    Port currentPort = station->portsList->head;
    while (currentPort) {
        if (currentPort->portType == car->portType && currentPort->status == FREE && currentPort->p2car == NULL) {
            compatiblePortAvailable = currentPort;
            break;
        }
        else if (currentPort->portType == car->portType && currentPort->status == OCCUPIED) {
            hasCompatiblePort = true; // Found a port that is occupied but compatible
        }
        currentPort = currentPort->next;
    }

    if (compatiblePortAvailable) {
        // Connect car to port
        compatiblePortAvailable->p2car = car;
        compatiblePortAvailable->status = OCCUPIED;
        car->pPort = compatiblePortAvailable;
        car->inqueue = 0;

        // Set start charging time
        Date currentDate = getCurrentDate();
        compatiblePortAvailable->tin->Year = currentDate->Year;
        compatiblePortAvailable->tin->Month = currentDate->Month;
        compatiblePortAvailable->tin->Day = currentDate->Day;
        compatiblePortAvailable->tin->Hour = currentDate->Hour;
        compatiblePortAvailable->tin->Min = currentDate->Min;

        printf("Car %s connected to port %d at station %d. Charging started at %04d-%02d-%02d %02d:%02d.\n",
            car->nLicense, compatiblePortAvailable->portType, stationID,
            compatiblePortAvailable->tin->Year, compatiblePortAvailable->tin->Month, compatiblePortAvailable->tin->Day,
            compatiblePortAvailable->tin->Hour, compatiblePortAvailable->tin->Min);
    }
    else if (hasCompatiblePort) {
        // Add car to station queue
        stationAddCarToQueue(station, car);
        car->inqueue = 1;
        car->pPort = NULL;
        printf("No compatible port available. Car %s added to the queue at station %d.\n", car->nLicense, stationID);
    }
    else {
        printf("No compatible port for car %s at station %d.\n", car->nLicense, stationID);
    }

    // Update files after charging
    updateFiles(stationTree, carTree); // Save stations and cars to files
}

void checkCarStatus(StationBST stationTree, CarBST carTree) {
    char license[PLATE_SIZE];
    printf("Enter car license number: ");
    if (scanf("%8s", license) != 1) {
        printf("Invalid input. Please enter a valid license.\n");
        while (getchar() != '\n'); // Clear input buffer
        return;
    }
	
    // Look up the car in the car BST by license
    Car car = findCarByLicense(carTree, license);
    if (!car) {
        printf("Car not found in the system.\n");
        return;
    }

    // Scan the station BST to check if car is currently charging
    Station station = stationTree->root;
    while (station) {
        Port port = station->portsList->head;
        while (port) {
            // Check if the car is currently being charged at this port
            if (port->p2car && strcmp(getCarLicense(port->p2car), license) == 0) {
                Date now = getCurrentDate();

                // Convert current date to time_t
                struct tm now_tm = { 0 };
                now_tm.tm_year = now->Year - 1900;
                now_tm.tm_mon = now->Month - 1;
                now_tm.tm_mday = now->Day;
                now_tm.tm_hour = now->Hour;
                now_tm.tm_min = now->Min;
                time_t now_time = mktime(&now_tm);

                // Convert charging start time to time_t
                struct tm start_tm = { 0 };
                start_tm.tm_year = port->tin->Year - 1900;
                start_tm.tm_mon = port->tin->Month - 1;
                start_tm.tm_mday = port->tin->Day;
                start_tm.tm_hour = port->tin->Hour;
                start_tm.tm_min = port->tin->Min;
                time_t start_time = mktime(&start_tm);

                // Calculate charging duration in minutes
                int minutes = (int)difftime(now_time, start_time) / 60;

                printf("Car is currently charging.\n");
                printf("Station: %s (ID: %d), Port: %d\n", station->name, station->id, port->num);
                printf("Charging Time: %d min\n", minutes);
                return;
            }
            port = port->next; // Move to next port
        }
        station = station->left ? station->left : station->right; // Traverse BST (naive, not recursive)
    }
    // Scan the station BST to check if car is in a queue
    station = stationTree->root;
    while (station) {
        CarNode node = station->carQueue->front;
        int position = 1;
        while (node) {
            // Check if car is found in this queue
            if (strcmp(getCarLicense(node->car), license) == 0) {
                printf("Car is waiting in queue.\n");
                printf("Station: %s (ID: %d), Queue Position: %d\n",
                    station->name, station->id, position);
                printf("Queue Type: ");
                printf("%s\n", portTypeToString(getCarPortType(car)));
                return;
            }
            node = node->next; // Move to next car in queue
            position++;
        }
        station = station->left ? station->left : station->right; // Traverse BST (naive, not recursive)
    }
    // Car is neither charging nor in a queue
    printf("Car is not currently charging or in a queue.\n");
}



void stopCharge(StationBST stationTree, CarBST carTree) {
    printf("stopCharge was called\n");
    if (!stationTree || !stationTree->root) return;

    char license[20];
    printf("Enter car license number: ");
    if (scanf("%8s", license) != 1) {
        printf("Invalid input. Please enter a valid license.\n");
        while (getchar() != '\n'); // Clear input buffer
        return;
    }
    Car car = findCar(carTree, license);
    if (!car) {
        printf("Car [%s] not found in system.\n", license);
        return;
    }

    int found = 0;
    Station stack[100];
    int top = -1;
    Station current = stationTree->root;

    while (current || top >= 0) {
        while (current) {
            stack[++top] = current;
            current = current->left;
        }
        current = stack[top--];

        PortList ports = current->portsList;
        Port p = listGetFirst(ports);
        while (p) {
            if (p->status == OCCUPIED && p->p2car) {
                const char* portLicense = getCarLicense(p->p2car);
                if (strcmp(portLicense, license) == 0) {
                    Date now = getCurrentDate();
                    int duration = abs(diffMinutes(p->tin, now));
                    double cost = duration * 1.2;
                    car->totalPayed += cost;
                    printf("Car [%s] stopped charging after %d minutes. Paid %.2f NIS.\n",
                        license, duration, cost);
                    p->status = FREE;
                    p->p2car = NULL;
                    destroyDate(p->tin);
                    p->tin = NULL;
                    if (current->nCars > 0) {
                        Car nextCar = qCarPeek(current->carQueue);
                        if (assignCar2port(nextCar, p)) {
                            setCarPort(nextCar, p);
                            setCarInQueue(nextCar, INQUEUE_NO);
                            qCarDequeue(current->carQueue);
                            current->nCars--;
                            printf("Next car [%s] started charging at port [%d]\n",
                                getCarLicense(nextCar), p->num);
                        }
                    }
                    found = 1;
                    break;
                }
            }
            p = listGetNext(ports, p);
        }
        if (found) break;
        current = current->right;
    }

    if (!found) {
        printf("Car [%s] is not currently charging.\n", license);
    }

    updateFiles(stationTree, carTree);
}

void dispAllSt(StationBST stationTree) {
    inorderTraversalStation(stationTree); // Display all stations in the binary search tree
}

void dispCarsAtSt(StationBST stationTree) {
    printf("dispCarsAtSt was called\n");

    Station mystation = searchStation(stationTree);
    if (!mystation) {
		displayError(STATION_NOT_FOUND);
        return;
    }

    printf("Currently charging:\n");
    Port port = mystation->portsList->head;
    int chargingCount = 0;
	Date now = getCurrentDate();
    while (port) {
        if (port->p2car) {
            Car car = port->p2car;
            int duration = diffMinutes(now,port->tin);
            printf("License: %s | Port Type: %s | Charging Time: %d minutes\n",
                getCarLicense(car),
                portTypeToString(getCarPortType(car)),
                duration);
            chargingCount++;
        }
        port = port->next;
    }
    if (chargingCount == 0) {
        printf("No cars currently charging.\n");
    }

    printf("Currently in Queue:\n");
    CarNode currentNode = mystation->carQueue->front;
    int queueCount = 0;
    while (currentNode) {
        Car car = currentNode->car;
        printf("License: %s | Port Type: %s\n",
            getCarLicense(car),
            portTypeToString(getCarPortType(car)));
        currentNode = currentNode->next;
        queueCount++;
    }
    if (queueCount == 0) {
        printf("No cars in queue.\n");
    }
}


void reportStStat(StationBST stationTree) {
    printf("reportStStat was called");
    Station station = searchStation(stationTree); //finds station
    if(!station) {
        displayError(STATION_NOT_FOUND);
        return;
	}
    else if (!station->portsList)
    {
		displayError(NO_PORTS);
        return;
    }
	// Calculate statistics
    int totalPorts = station->nPorts;
    int activeP = 0;
    int brokenP = 0;
    int freeP = 0;
    int busyP = 0;

    Port port = station->portsList->head;
    while (port) {
        if (port->status == OUT_OF_ORDER)
            brokenP++;
        else if (port->status == FREE)
            freeP++;
        else if (port->status == OCCUPIED) {
            busyP++;
            activeP++;
        }
        port = port->next;
    }

    double utilization = (totalPorts > 0) ? (100.0 * busyP / totalPorts) : 0.0;
    double brokenRatio = (totalPorts > 0) ? (100.0 * brokenP / totalPorts) : 0.0;
    double loadRatio = (station->nCars + busyP > 0) ? (double)busyP / (station->nCars) : 0.0;

    // Print statistics
    printf("--- Station Status Report ---\n");
    printf("Station Name: %s (ID: %d)\n", station->name, station->id);
    printf("Total Ports: %d\n", totalPorts);
    printf("Utilization: %.2f%%\n", utilization);
    printf("Broken Ports Ratio: %.2f%%\n", brokenRatio);
    printf("Current Load Ratio: %.2f\n", loadRatio);

    // Print load level description
    if (loadRatio > 1.0 || stationTree->root->carQueue->front == NULL)
        printf("Load Status: No load\n");
    else if (loadRatio == 1.0)
        printf("Load Status: Normal Load\n");
    else if (loadRatio < 1 && loadRatio >= 0.2)
        printf("Load Status: Loaded\n");
    else
        printf("Load Status: Heavy Loaded\n");
}

void dispTopCostumers(CarBST carTree) {
    printf("dispTopCostumers was called\n");
    if (!carTree || !carTree->root) {
        displayError(CAR_NOT_FOUND);
        return;
    }

    Car topCars[5] = { NULL }; // Store top 5 paying customers

    int height = getTreeHeight(carTree->root); // Dynamically get height
    tCar* stack = malloc(sizeof(tCar) * height);
    if (!stack) {
		displayError(MEMORY_ALOC_FAILED);
        return;
    }

    int top = -1;
    tCar current = carTree->root;

    while (current || top != -1) {
        while (current) {
            stack[++top] = current;
            current = current->left;
        }

        current = stack[top--];
        Car car = current->p2car;
        double paid = getCarTotalPayed(car);

        for (int i = 0; i < 5; ++i) {
            if (!topCars[i] || paid > getCarTotalPayed(topCars[i])) {
                for (int j = 4; j > i; --j) {
                    topCars[j] = topCars[j - 1];
                }
                topCars[i] = car;
                break;
            }
        }

        current = current->right;
    }

    free(stack);

    printf("Top 5 paying customers:\n");
    for (int i = 0; i < 5 && topCars[i]; ++i) {
        printf("%d. License: %s | Total Paid: %.2f\n",
            i + 1,
            getCarLicense(topCars[i]),
            getCarTotalPayed(topCars[i])
        );
    }
}

void addNewPort(StationBST stationTree) {
    printf("addNewPort was called\n");
    int choice = 0;
    Station station = searchStation(stationTree);
    while (!station)
    {
        printf("Try again\n");
        station = searchStation(stationTree);
    }
    int typeChoice = -1;
    while (typeChoice < 0 || typeChoice > 2) {
        printf("Enter new Port type :\n");
        printf("0 - FAST, 1 - MID, 2 - SLOW \n ");

        if (scanf("%d", &typeChoice) != 1 || typeChoice < 0 || typeChoice > 2) {
			displayError(INVALID_INPUT);
            while (getchar() != '\n'); //clears buffer if needed
            typeChoice = -1;
        }
    }
    PortType newtype = (PortType)typeChoice;

    int portNum = listGetSize(station->portsList) + 1;
    Port newPort = createPort(portNum, newtype, FREE, 0, 0, 0, 0, 0);

    if (!newPort) {
        printf("Error creating new Port\n");
        return;
    }

    stationAddPort(station, newPort);
    station->nPorts++;

    printf("Your port added succesfully\n");

    // check if there is car in queue that can start charging in the new port
    int queueSize = qCarSize(station->carQueue);
    for (int i = 0; i < queueSize; i++) {
        Car nextCar = qCarDequeue(station->carQueue);
        if (getCarPortType(nextCar) == newtype) {
            newPort->p2car = nextCar;
            newPort->status = OCCUPIED;
            station->nCars--;
            newPort->tin = getCurrentDate();
            setCarPort(nextCar, newPort);
            setCarInQueue(nextCar, INQUEUE_NO);
            printf("car number %s started charging in new port from type %s \n",
                getCarLicense(nextCar), portTypeToString(newtype));
            return;
        }
        else {
            qCarEnqueue(station->carQueue, nextCar);
        }
    }
    printf("no car with compatible port in queue.\n");
}

void releasePorts(StationBST stationTree) {
    printf("releasePorts was called\n");
    if (!stationTree || !stationTree->root) {
		displayError(EMPTY_BST); 
        return;
    }

    Station station = searchStation(stationTree);
    if (!station) {
		displayError(STATION_NOT_FOUND);
        return;
    }

    Port port = listGetFirst(station->portsList);
    int releasedCount = 0;
    Date now = getCurrentDate();

    while (port) {
        if (port->status == OCCUPIED)// checks all cars that connected to ports
        {
            Date tin = port->tin;
            double hours = (now->Year - tin->Year) * 8760 +
                (now->Month - tin->Month) * 730 +
                (now->Day - tin->Day) * 24 +
                (now->Hour - tin->Hour) +
                (now->Min - tin->Min) / 60.0;

            if (hours >= 10) {
                Car car = port->p2car;
                printf("Car with license %s disconnected from port (%.2f hours).\n", getCarLicense(car), hours);

                // Disconnect
                port->p2car = NULL;
                port->status = FREE;

                setCarPort(car, NULL);
                setCarInQueue(car, INQUEUE_NO);

                // Check queue for compatible car
                int queueSize = qCarSize(station->carQueue);
                for (int i = 0; i < queueSize; i++) {
                    Car nextCar = qCarDequeue(station->carQueue);
                    if (getCarPortType(nextCar) == port->portType) {
                        port->p2car = nextCar;
                        port->status = OCCUPIED;
                        port->tin = getCurrentDate();
                        setCarPort(nextCar, port);
                        setCarInQueue(nextCar, INQUEUE_NO);
                        printf("     -> Car with license %s connected to port from type %s.\n", getCarLicense(nextCar), portTypeToString(port->portType));
                        break;
                    }
                    else {
                        qCarEnqueue(station->carQueue, nextCar);
                    }
                }
                releasedCount++;
            }
        }
        port = listGetNext(station->portsList, port);
    }

    if (releasedCount == 0) {
        printf("No cars exceeded the maximum charging time in the selected station.\n");
    }
}

void remOutOrderPort(StationBST stationTree) {
    printf("remOutOrderPort was called\n");
    if (!stationTree || !stationTree->root) {
		displayError(EMPTY_BST);
        return;
    }

    int stationId;
    printf("Enter station ID: ");
    if (scanf("%d", &stationId) != 1) {
		displayError(INVALID_INPUT);
        return;
    }

    Station station = searchStationByID(stationTree, stationId);
    if (!station) {
        printf("Station with ID %d not found\n", stationId);
        return;
    }

    // Print out-of-order ports
    Port port = listGetFirst(station->portsList);
    int found = 0;
    printf("Out of order ports at station %d:\n", stationId);
    while (port) {
        if (port->status == OUT_OF_ORDER) {
            printf("Port #%d (Type: %s)\n", port->num, portTypeToString(port->portType));
            found++;
        }
        port = listGetNext(station->portsList, port);
    }

    if (found == 0) {
        printf("No out oforder ports at this station\n");
        return;
    }

    int portNum;
    printf("Enter port number to remove: ");
    if (scanf("%d", &portNum) != 1) {
		displayError(INVALID_INPUT);
        return;
    }

    port = listGetFirst(station->portsList);
    while (port) {
        if (port->num == portNum && port->status == OUT_OF_ORDER) {
            listRemovePort(station->portsList, port);
            station->nPorts--;
            printf("  Port #%d removed successfully from station %d\n", portNum, stationId);
            return;
        }
        port = listGetNext(station->portsList, port);
    }

    printf("Port #%d is not out-of-order or does not exist in station %d\n", portNum, stationId);
}

void remCustomer(CarBST carTree) {
    printf("remCustomer was called\n");
    char license[PLATE_SIZE];
    printf("Enter license number to remove: ");
    if (scanf("%8s", license) != 1) {
        displayError(INVALID_INPUT);
        while (getchar() != '\n'); // Clear input buffer
        return;
    }  
    Car car = findCarByLicense(carTree, license);
    if (!car) {
        printf("Car with license [%s] not found.\n", license);
        return;
    }
    if (getCarPort(car) != NULL) {
        printf("Cannot remove car [%s]: currently charging.\n", license);
        return;
    }
    if (isCarInQueue(car) == INQUEUE_YES) {
        printf("Cannot remove car [%s]: currently in queue.\n", license);
        return;
    }
    removeCar(carTree, car);     // removes from BST
	carDestroy(car);             // freeing memory
    printf("Car [%s] was successfully removed.\n", license);
}

void closeSt(StationBST stationTree) {
    printf("closeSt was called\n");
    if (!stationTree) return;

    int choice;
    Station stationToClose = NULL;

    printf("Close station by:\n1. ID\n2. Name\nChoose option: ");
    if (scanf("%d", &choice) != 1) {
		displayError(INVALID_INPUT);
        while (getchar() != '\n');
        return;
    }

    if (choice == 1) {
        int id;
        printf("Enter station ID to close: ");
        if (scanf("%d", &id) != 1) {
            displayError(INVALID_INPUT);
            while (getchar() != '\n');
            return;
        }
        stationToClose = searchStationByID(stationTree, id);
    }
    else if (choice == 2) {
        char name[100];
        printf("Enter station name to close: ");
        while (getchar() != '\n');
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = '\0';
        stationToClose = searchStationByName(stationTree, name);
    }
    else {
        displayError(INVALID_INPUT);
        return;
    }

    if (!stationToClose) {
		displayError(STATION_NOT_FOUND);
        return;
    }

    char* nameCopy = _strdup(stationToClose->name); //saves name before deleting station

    Station temp = stationCreate(
        stationToClose->id,
        stationToClose->name,
        stationToClose->nPorts,
        stationToClose->coord.x,
        stationToClose->coord.y
    );

    deleteStation(stationTree, temp);  //station delete
    stationDestroy(temp);// freeing memory of copy

    printf("Station \"%s\" closed and removed from the system.\n", nameCopy);
	free(nameCopy); //free the name copy


}

void exitSys(StationBST stationTree, CarBST carTree) {
    // printf("exitSys was called, updating files...\n");
    // update the files before exiting
    updateFiles(stationTree, carTree); // Update files with the current state of the system
    // Free resources and exit the program
    freeStationBst(stationTree); // Free the memory allocated for the station tree
    freeCarBST(carTree); // Free the memory allocated for the car tree
    printf("###############################################\n");
    printf("#          Exiting system. Goodbye!           #\n");
    printf("###############################################\n");
}


/** Helper Functions */

void inserPortFromLine(const char* line, StationBST stationTree, CarBST carTree) {
    if (!stationTree || !line) return; // Check for null pointers

    int stationID, portNumber;
    char portTypeStr[16]; // Assuming PortType is a string representation
    PortType portType;
    int year, month, day, hour, minute;
    int tempStatus;
    char carLicense[9]; // Assuming PortStatus is a string representation

    // Parse the line
    int parsed = sscanf(line, "%d,%d,%5[^,],%d,%d,%d,%d,%d,%d,%8[^,]", &stationID, &portNumber, portTypeStr, &tempStatus, &year, &month, &day, &hour, &minute, carLicense);
    if (parsed != 10) {
        printf("Invalid port line format: %s\n", line);
        return;
    }

    // Find the station by ID
    Station station = searchStationByID(stationTree, stationID);
    if (!station) {
		displayError(STATION_NOT_FOUND);
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
        printf("Invalid port type: %s\n", portTypeStr);
        return; // Invalid port type    
    }

    Port newPort = createPort(portNumber, portType, (PortStatus)tempStatus, year, month, day, hour, minute);
    if (!newPort) {
		displayError(FAILED_CREATE_PORT);
        return;
    }

    carLicense[strcspn(carLicense, "\r\n")] = '\0'; // remove newline
    if (strcmp(carLicense, "-1") != 0) {
        // printf("GOT HERE!!\n");
        Car car = findCarByLicense(carTree, carLicense);
        if (!car) {
            displayError(CAR_NOT_FOUND);
            return;
        }
        newPort->p2car = car; // Assign the car to the port
        car->pPort = newPort; // Assign the port to the car
    }

    listInsertLast(station->portsList, newPort); // Insert the new port into the station's port list
}

void loadPortsFromFile(const char* filename, StationBST stationTree, CarBST carTree) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open %s\n", filename);
        return;
    }
    char line[256];
    // Skip the first line (header)
    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        return; // Skip if the file is empty or only has a header
    }
    while (fgets(line, sizeof(line), file)) {
        inserPortFromLine(line, stationTree, carTree); // Assuming this function exists to handle the line
    }
    fclose(file);
}


void loadLineOfCarsfromLine(const char* line, StationBST stationTree, CarBST carTree) {
    if (!stationTree || !carTree || !line) return; // Check for null pointers

    char license[PLATE_SIZE];
    int stationID;

    // Parse the line
    int parsed = sscanf(line, "%8[^,],%d", license, &stationID);
    if (parsed != 2) {
        printf("Invalid car line format: %s\n", line);
        return;
    }
    // Find the station by ID
    Station station = searchStationByID(stationTree, stationID);
    if (!station) {
        displayError(STATION_NOT_FOUND);
        return;
    }
    // Find the car by license
    Car car = findCarByLicense(carTree, license);
    if (!car) {
        displayError(CAR_NOT_FOUND);
        return;
    }
    // Add the car to the station's queue
    stationAddCarToQueue(station, car);
}

void loadLineOfCarsFromFile(const char* filename, StationBST stationTree, CarBST carTree) {
    // printf("Loading cars from file... %s\n", filename);
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open %s\n", filename);
        return;
    }
    char line[256];
    // Skip the first line (header)
    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        return; // Skip if the file is empty or only has a header
    }
    while (fgets(line, sizeof(line), file)) {
        loadLineOfCarsfromLine(line, stationTree, carTree); // Assuming this function exists to handle the line
    }
    fclose(file);
}



void loadFiles(StationBST stationTree, CarBST carTree) {
    if (!stationTree || !carTree) {
        displayError(EMPTY_BST);
        return; // Exit if trees are not created
    }
    // Load data from Stations.txt
    printf("Loading stations from Stations.txt...\n");
    loadStationsFromFile(stationTree, "Stations.txt");
    printf("Station data loaded successfully!\n");

    // Load data from Cars.txt
    printf("Loading cars from Cars.txt...\n");
    loadCarsFromFile(carTree, stationTree, "Cars.txt");
    printf("Car data loaded successfully!\n");

    // Load data from Ports.txt
    printf("Loading ports from Ports.txt...\n");
    loadPortsFromFile("Ports.txt", stationTree, carTree);
    printf("Ports loaded successfully!\n");

    // Load data from LineOfCars.txt
    printf("Loading line of cars from LineOfCars.txt...\n");
    loadLineOfCarsFromFile("LineOfCars.txt", stationTree, carTree);
    printf("Line of cars loaded successfully!\n\n");
}



void savePortsToFileRecursive(Station root, FILE* file) {
    if (root) {
        savePortsToFileRecursive(root->left, file); // Traverse left subtree
        PortList portsList = root->portsList;
        if (!portsList || !portsList->head) {
            displayError(NO_PORTS);

        }
        else {
            // Save each port in the list to the file
            Port currentPort = portsList->head;
            while (currentPort) {
                fprintf(file, "%d,%d,%s,%d,%d,%d,%d,%d,%d,%s\n",
                    root->id, currentPort->num,
                    (currentPort->portType == FAST) ? "FAST" :
                    (currentPort->portType == MID) ? "MID" : "SLOW",
                    currentPort->status,
                    currentPort->tin ? currentPort->tin->Year : 0,
                    currentPort->tin ? currentPort->tin->Month : 0,
                    currentPort->tin ? currentPort->tin->Day : 0,
                    currentPort->tin ? currentPort->tin->Hour : 0,
                    currentPort->tin ? currentPort->tin->Min : 0,
                    currentPort->p2car ? currentPort->p2car->nLicense : "-1");
                currentPort = currentPort->next;
            }
        }
        savePortsToFileRecursive(root->right, file); // Traverse right subtree
    }
}


void savePortsToFile(StationBST stationTree, const char* filename) {
    if (!stationTree || !filename) {
        printf("Error: Station tree or filename is NULL.\n");
        return;
    }
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Failed to open file: %s for saving ports\n", filename);
        return;
    }
    // Write the header
    fprintf(file, "StationID,PortNumber,PortType,Status,Year,Month,Day,Hour,Min,CarLicense\n");
    // Inorder traversal to write each port
    Station currentStation = stationTree->root;
    if (currentStation) {
        // Traverse the tree and save ports
        savePortsToFileRecursive(currentStation, file);
    }
    fclose(file); // Close the file
    printf("Ports saved to %s\n", filename);
}

void saveLineOfCarsToFileRecursive(Station root, FILE* file) {
    if (root) {
        // Traverse the left subtree
        saveLineOfCarsToFileRecursive(root->left, file);
        // Save the line of cars for the current station
        if (root->carQueue) {
            CarNode node = root->carQueue->front;
            while (node) {
                fprintf(file, "%s,%d\n", node->car->nLicense, root->id);
                node = node->next; // Move to the next car in the queue
            }
        }
        // Traverse the right subtree
        saveLineOfCarsToFileRecursive(root->right, file);
    }
}


void saveLineOfCarsToFile(StationBST stationTree, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Failed to open file: %s for saving line of cars\n", filename);
        return;
    }
    fprintf(file, "License,StationID\n"); // Write the header
    saveLineOfCarsToFileRecursive(stationTree->root, file);
    fclose(file);
    printf("Line of cars saved to %s\n", filename);
}


void inorder_save_cars_to_file(StationBST stationsTree, tCar root, FILE* file) {
    if (root) {
        inorder_save_cars_to_file(stationsTree, root->left, file);
        Station station = NULL;
        // If the car has a port, find the station by the port
        if (root->p2car->pPort) {
            // Find the station by the port
            station = searchStationByPortPointer(stationsTree, root->p2car->pPort);
        }
        else if (root->p2car->inqueue == TRUE) { // Should be true only if port is null
            station = searchStationByCarLicenseInQueue(stationsTree, root->p2car->nLicense);

        }
        fprintf(file, "%s,%s,%.2lf,%d,%d,%u\n",
            root->p2car->nLicense,
            (root->p2car->portType == FAST) ? "FAST" :
            (root->p2car->portType == MID) ? "MID" : "SLOW",
            root->p2car->totalPayed,
            station ? station->id : 0, // Assuming Port has a stationID field
            root->p2car->pPort ? root->p2car->pPort->num : 0, // Assuming Port has an id field
            root->p2car->inqueue);
        inorder_save_cars_to_file(stationsTree, root->right, file);
    }
}


void saveCarsToFile(StationBST stationTree, CarBST carTree, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Failed to open file: %s\n", filename);
        return;
    }
    // Write the header
    fprintf(file, "License,PortType,TotalPaid,StationID,PortNumber,InQueue\n");
    // Inorder traversal to write each car
    inorder_save_cars_to_file(stationTree, carTree->root, file);
    fclose(file);
    printf("Cars saved to %s\n", filename);
}


void updateFiles(StationBST stationTree, CarBST carTree) {
    if (!stationTree || !carTree) {
        displayError(EMPTY_BST);
        return; // Exit if trees are not created
    }

    // Save stations to file
    saveStationsToFile(stationTree, "Stations.txt"); 

    // Save ports to file
    savePortsToFile(stationTree, "Ports.txt"); 
    // Save cars to file
    saveCarsToFile(stationTree, carTree, "Cars.txt"); 
    // Save line of cars to file
    saveLineOfCarsToFile(stationTree, "LineOfCars.txt"); 

    printf("Files updated successfully!\n");
}



Car searchCar(CarBST carTree, const char* license) {
    if (!carTree || !license) return NULL; // Check for null pointers
    return findCarByLicense(carTree, license); // Assuming this function exists to search by license
}

/** Search station helper functions */
// Function pointer types
typedef Station(*SearchFunc)(StationBST tree, void* arg);

// Wrappers to match function pointer signature
Station searchByIDWrapper(StationBST tree, void* arg) {
    return searchStationByID(tree, *(int*)arg);
}

Station searchByNameWrapper(StationBST tree, void* arg) {
    return searchStationByName(tree, (char*)arg);
}

Station searchByDistanceWrapper(StationBST tree, void* arg) {
    double* coords = (double*)arg;
    return searchNearestStationByCoordinates(tree, coords[0], coords[1]);
}

Station searchStation(StationBST stationTree) {  
    if (!stationTree || !stationTree->root) {  
        displayError(STATION_NOT_FOUND);
        return NULL;
    }  

    printf("Choose search method:\n");  
    printf("1. By ID\n");  
    printf("2. By Name\n");  
    printf("3. By Distance to Your Location\n");  
    printf("Enter your choice (1-3): ");  

    int choice;  
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > 3) {  
        displayError(INVALID_INPUT);
        while (getchar() != '\n'); // flush buffer  
        return NULL;  
    }  

    // Function pointer array  
    SearchFunc searchMethods[3] = { searchByIDWrapper, searchByNameWrapper, searchByDistanceWrapper };  

    // Prepare the right argument  
    void* arg = NULL;  
    int id;  
    char name[NAME_SIZE];  
    double coords[2];  

    switch (choice) {  
    case 1:  
        printf("Enter station ID: ");  
        if (scanf("%d", &id) != 1) {  
            displayError(INVALID_INPUT);
            while (getchar() != '\n');  
            return NULL;  
        }  
        arg = &id;  
        break;  
    case 2:  
        printf("Enter station name: ");  
        while (getchar() != '\n'); // clear leftover newline  
        fgets(name, NAME_SIZE, stdin);  
        name[strcspn(name, "\n")] = 0; // remove newline  
        arg = name;  
        break;  
    case 3:  
        printf("Enter your coordinates (x y): ");  
        if (scanf("%lf %lf", &coords[0], &coords[1]) != 2) {  
            displayError(INVALID_INPUT);
            while (getchar() != '\n');  
            return NULL;  
        }  
        arg = coords;  
        break;  
    }  

    Station result = searchMethods[choice - 1](stationTree, arg);  

    if (result) {  
        printf("Station found:\nID: %d\nName: %s\n", result->id, result->name);  
    } else {  
        displayError(STATION_NOT_FOUND);
    }  

    return result;  
}

void displayError(ErrorCode code) //usng this function to display error messages based on the error code
{
    switch (code) {
    case INVALID_INPUT:
        printf("Error: Invalid input,please try again.\n");
        break;
    case STATION_NOT_FOUND:
        printf("Error: Station not found.\n");
        break;
    case PORT_NOT_FOUND:
        printf("Error: Port not found.\n");
        break;
    case CAR_NOT_FOUND:
        printf("Error: Car not found.\n");
        break;
    case MEMORY_ALOC_FAILED:
        printf("Error: not enough memory.\n");
        break;
    case EMPTY_BST:
        printf("Error:your BST is empty.\n");
        break;
    case FILE_OPEN_ERROR:
        printf("Error:Canot load your file.\n");
        break;
    case EMPTY_INPUT:
        printf("Error:the input that sent is empty.\n");
    case NO_PORTS:
        printf("Error:this station have no ports avilable.\n");
        break;
    case FAILED_CREATE_PORT:
        printf("Error:failed to create port.\n");
        break;
    case INPUT_MENU:
        printf("Invalid choice. Please select a number between 1 and 13.\n");
        break;
    default:
        printf("Unknown error occurred.\n");
    }
}

void handleUserChoice(CarBST carTree, StationBST stationTree) {
    int choice;
    bool running = true;

    while (running) {
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            displayError(INVALID_INPUT);
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        switch (choice) {
        case 1:
			locNearSt(stationTree); //locates the nearest station to the user's coordinates
            break;
        case 2:
			chargeCar(stationTree, carTree); //charge a car 
            break;
        case 3: 
			checkCarStatus(stationTree, carTree);// Check the status of a car
            break;
        case 4:
			stopCharge(stationTree, carTree);// Stop charging a car
            break;
        case 5:
            dispAllSt(stationTree); // Display all stations in the binary search tree
            break;
        case 6: 
			dispCarsAtSt(stationTree);// Display all cars at a spesific station
            break;
        case 7: 
			reportStStat(stationTree);//prints the load level of the station (includes %)
            break;
        case 8: 
			dispTopCostumers(carTree);//prints top 5 paying customers
            break;
        case 9:
			addNewPort(stationTree);
            updateFiles(stationTree, carTree); // Update files with the current state of the system// Add a new port to a station
            break;
        case 10:
			releasePorts(stationTree);
            updateFiles(stationTree, carTree); // Update files with the current state of the system// Release ports that have been occupied for more than 10 hours
            break;
        case 11:
			remOutOrderPort(stationTree);
            updateFiles(stationTree, carTree); // Update files with the current state of the system// Remove an out-of-order port from a station
            break;
        case 12:
			remCustomer(carTree);
            updateFiles(stationTree, carTree); // Update files with the current state of the system// Remove a customer (car) from the system
            break;
        case 13:
			closeSt(stationTree);
            updateFiles(stationTree, carTree); // Update files with the current state of the system// Close a station and remove it from the system
            break;
        case 0:
			exitSys(stationTree, carTree);// Exit the system and update files
            running = false;
            break;
   //     case 15: //TODO: remove after checks
   //         printf("Updating files...\n");
			//updateFiles(stationTree, carTree); // Update files with the current state of the system
   //         break;
   //     case 16: //TODO: remove after checks
   //         printf("Searching for a station...\n");
   //         searchStation(stationTree); // Search for a station by ID, name, or coordinates
   //         break;
        default:
			displayError(INPUT_MENU); // Display error for invalid input
            break;
        }
    }
}



int main() {

    printf("Welcome to the Charging Station Management System!\n\n");
    printf("Loading data...\n");

    // load data from Stations.txt
    StationBST stationTree = create_station_bst();
    if (!stationTree) {
        printf("Failed to create station binary search tree.\n");
        return 1; // Exit if tree creation failed
    }
    CarBST carTree = create_car_bst();
    if (!carTree) {
        printf("Failed to create car binary search tree.\n");
        return 1; // Exit if tree creation failed
    }

    // Load data from files
    loadFiles(stationTree, carTree);

    print_menu();
    handleUserChoice(carTree, stationTree); // Handle user choices based on the menu


    return 0;
}

