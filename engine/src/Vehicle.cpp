#include "Vehicle.h"

#include <iostream>
using namespace std;

Vehicle::Vehicle() {
	cout << "Creating vehicle" << endl;
}

Vehicle::~Vehicle() {
	cout << "Destroying vehicle" << endl;
}