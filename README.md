# ⚡ EV Charging Station Management System

A comprehensive C-based software system for managing electric vehicle (EV) charging stations. 
Developed as a final project by Ron Yona Cohen and Shon Gottfried.

## 🚀 Key Features
* **Smart Charging Management:** Connects cars to Fast, Mid, or Slow charging ports based on availability.
* **Queue System:** Automatically handles waiting lines for occupied stations.
* **Real-time Statistics:** Generates load levels, utilization percentages, and broken port ratios.
* **Data Persistence:** Fully supports reading and writing to files (`Stations.txt`, `Cars.txt`, etc.) so no data is lost between sessions.

## 🧠 Technical Highlights
This project heavily utilizes dynamic memory allocation and complex data structures:
* **Binary Search Trees (BST):** Used for efficient searching and managing of Stations and Cars.
* **Linked Lists:** Implemented to track the charging ports at each station.
* **Queues:** Used to manage cars waiting for an available port.

## 🛠️ How to Run
1. Clone the repository.
2. Compile all `.c` files using a standard C compiler (e.g., GCC).
3. Run the executable.
4. Follow the on-screen terminal menu to interact with the system.
