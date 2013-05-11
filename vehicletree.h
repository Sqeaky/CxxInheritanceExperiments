// ©2013 Sqeaky
// This is distibuted under the WTFPL, see COPYING.md for details.

#ifndef vehicletree_h
#define vehicletree_h

#include <iostream>
#include <cstring>

// Normally including this in a header is rude, but this is intended only to make the other files
// in this group of experiments easier to read.
using namespace std;

// This example was pulled directly from http://www.parashift.com/c++-faq/derivedptrptr-to-baseptrptr.html
//
class Vehicle {
    public:
        virtual ~Vehicle()
            {}

        virtual void startEngine() = 0;
};

class Car : public Vehicle {
    public:
        virtual void startEngine()
            { cout << "Start a car." << endl; }

        virtual void openGasCap()
            { cout << "Open a Car's gas cap." << endl; }
};

class NuclearSubmarine : public Vehicle {
    public:
        virtual void startEngine()
            { cout << "Start a NuclearSubmarine engine." << endl; }

        virtual void fireNuclearMissle()
            { cout << "Fire the MISSILES!!!!" << endl; }
};

// In this test the car branch of the vehicle hiearchy will serve as the targets for some very basic smart pointer proxies.
class Chevy : public Car {
    public:
        virtual void startEngine()
            { cout << "Start a Chevy." << endl; }

        virtual void openGasCap()
            { cout << "Open a Chevy's gas cap." << endl; }
};


#endif //vehicletree_h
