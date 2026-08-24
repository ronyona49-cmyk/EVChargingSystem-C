#include "portList.h"

PortList listCreate()//creats list
{
    PortList list = malloc(sizeof(struct portList_t));
	if (!list)// Handle memory allocation failure
    {
        printf("Memorry allocation failed.\n");
        return NULL;
    }
	// Initialize the list
    list->head = list->current = NULL;
    return list;
}

PortList listCopy(PortList list)// create a list copy,copying ports without copying assigned car (p2car is set to NULL)

{
	if (!list)// Check for null pointer
    {
        return NULL;
    }
    PortList newList = listCreate();
    if (!newList) return NULL;
	// Iterate through the original list and copy each port
    Port curr = list->head;
    while (curr) {
        Port newPort = createPort(curr->num, curr->portType, curr->status, curr->tin->Year, curr->tin->Month, curr->tin->Day, curr->tin->Hour, curr->tin->Min);
        if (!newPort || !listInsertLast(newList, newPort)) //if memry allocation failed or insertion failed
        {
            listDestroy(newList);
            return NULL;
        }
        curr = curr->next;
    }
    return newList;
}

void listDestroy(PortList list) //destroys list and freeing memory
{
	if (!list) return; // Check for null pointer
	// Free all ports in the list
    Port curr = list->head;
    while (curr) {
        Port next = curr->next;
        destroyDate(curr->tin);
        free(curr);
        curr = next;
    }
    free(list);
}

Port listGetFirst(PortList list) 
{
	if (!list || !list->head) return NULL; // Check for null pointer or empty list
    return list->head;
}

Port listFindPort(PortList list, Port port)//finds port 
{
    if (!list || !port) return NULL;
	// Iterate through the list to find the port
    Port curr = list->head;
    while (curr) {
        if (curr == port) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL; // Not found
}

Port listInsertFirst(PortList list, Port port) 
{
	if (!list || !port)  return NULL;// Check for null pointers
	// Create a new port node with the same properties as the given port
    Port newNode = createPort(port->num, port->portType, port->status, port->tin->Year, port->tin->Month, port->tin->Day, port->tin->Hour, port->tin->Min);
    if (!newNode) return NULL; // Memory allocation failed, error message in the createPort func
    newNode->next = list->head;
    list->head = newNode;
    if (!list->current) {
        list->current = newNode; // Set current to the new head
    }
    return port;
}

Port listInsertLast(PortList list, Port port)
{
	if (!list || !port)  return NULL;// Check for null pointers
	// Create a new port node with the same properties as the given port
    port->next = NULL;

	if (!list->head) // If the list is empty, set the new port as the head
    {
        list->head = port;
        list->current = port; // Set current to the new head
    }
	else // If the list is not empty, find the last port and append the new port
    {
        Port temp = list->head;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = port;
    }
    return port;
}

Port listRemovePort(PortList list, Port port)// removes port
{
    if (!list || !port) return NULL;
	// Iterate through the list to find the port to remove
    Port prev = NULL;
    Port curr = list->head;
	// If the port to remove is the head, update the head pointer
    while (curr) {
        if (curr == port) {
			if (prev)// If the port to remove is not the head, update the previous port's next pointer
            {
                prev->next = curr->next;
            }
			else// If the port to remove is the head, update the head pointer
            {
                list->head = curr->next; // Remove head
            }
            if (list->current == curr)
            {
                list->current = prev ? prev : list->head; // Update current
            }
            Port removedPort = curr;
            destroyPort(curr);
            return removedPort;
        }
        prev = curr;
        curr = curr->next;
    }
    return list->head;
}

Port listGetNext(PortList list, Port port)
{
	if (!list || !port)  return NULL;// Check for null pointers
	// Iterate through the list to find the next port after the given port
    Port curr = list->head;
    while (curr) {
        if (curr == port) {
            return curr->next; // Return the next port
        }
        curr = curr->next;
    }
    return NULL; // Not found
}

int listGetSize(PortList list)// returns the size of list
{
	if (!list) return 0;// Check for null pointer
	// If the list is empty, return 0
    int size = 0;
    Port curr = list->head;
    while (curr) {
        size++;
        curr = curr->next;
    }
    return size;
}

bool listIsEmpty(PortList list)//checks if list is empty
{
    return !list || !list->head;
}

void listClear(PortList list) {
	if (!list) return; // Check for null pointer
	// Free all ports in the list and reset the list
    Port curr = list->head;
    while (curr) {
        Port next = curr->next;
        destroyPort(curr); // Free the port and the date associated with it
        curr = next;
    }
    list->head = list->current = NULL; // Reset the list
}

Port createPort(int num, PortType type, PortStatus status, int year, int month, int day, int hour, int minute)//creates new port
{
    Port newPort = malloc(sizeof(struct port_t));
	if (!newPort)// Handle memory allocation failure
    {
        printf("Memory allocation failed.\n");
        return NULL;
    }
	// Initialize the port's fields
    newPort->num = num;
    newPort->portType = type;
    newPort->status = status; // Default status
    newPort->p2car = NULL; // No car initially
    newPort->tin = createDate(year, month, day, hour, minute); // Create date for charging start time
    newPort->next = NULL;   // No next port in the list initially
    return newPort;
}

void destroyPort(Port port)//delete port and free memory 
{
    if (port) {
        destroyDate(port->tin); // Free the date associated with the port
        free(port);             // Free the port structure itself
    }
}

const char* portTypeToString(PortType type)// converts the port type to string
{
    switch (type) {
    case FAST: return "FAST";
    case MID: return "MID";
    case SLOW: return "SLOW";
    default: return "UNKNOWN";
    }
}

void listPrint(PortList list) 
{
	if (!list || !list->head) return;// Check for null pointer or empty list
	// Print the header
    Port curr = list->head;
    while (curr) {
        printf("Port Number: %d, Type: %s, Status: %d, Car License: %s, Start Time: %d-%02d-%02d %02d:%02d\n",
            curr->num,
            portTypeToString(curr->portType),
            curr->status,
            curr->p2car ? getCarLicense(curr->p2car) : "None",
            curr->tin->Year, curr->tin->Month, curr->tin->Day, curr->tin->Hour, curr->tin->Min);
        curr = curr->next;
    }
}

void listPrintIds(PortList list)
{
    if (!list || !list->head) return; 
	// Print the IDs of the ports in the list
    Port curr = list->head;
    printf("PortsList IDs: ");
    while (curr) {
        if (curr->p2car) {
            printf("[PortID: %d, Car License: %s] -> ", curr->num, getCarLicense(curr->p2car));
        }
        else {
            printf("[PortID: %d, No Car] -> ", curr->num);
        }
        curr = curr->next;
    }
    printf("NULL\n");
}

bool assignCar2port(Car car, Port port)//assigns car to specific port
// Overwrites previous tin if exists. car->pPort is NOT updated here.
{
    if (!car || !port) return false; 
	if (port->status == OCCUPIED)// Check if the port is already occupied
    {
        printf("Port %d is already occupied\n", port->num);
        return false;
    }
    port->p2car = car;
	setCarPort(car, port); // Set the car's port to this port
    port->status = OCCUPIED;

    if (port->tin) 
        destroyDate(port->tin);

    port->tin = getCurrentDate();

    printf("Assigned car [%s] to port [%d]. Charging started at ", getCarLicense(car), port->num);
    printDate(port->tin);

    return true;
}

