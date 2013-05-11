// ©2013 Sqeaky
// This is distibuted under the WTFPL, see COPYING.md for details.

#ifndef missile_h
#define missile_h

#include "vehicletree.h"


///////////////////////////////////////////////////////////////////////////
// Really I just want opening the gas cap to fire the missiles

// This example was pulled directly from http://www.parashift.com/c++-faq/derivedptrptr-to-baseptrptr.html
void OpenGasCapToFireMissiles()
{
    cout << "Creating Car pointer pointer." << endl;
    Car   car;
    Car*  carPtr = &car;
    Car** carPtrPtr = &carPtr;

    //error: invalid conversion from âCar**â to âVehicle**â [-fpermissive]
    //Vehicle** vehiclePtrPtr = carPtrPtr;  // This is an error in C++
    Vehicle** vehiclePtrPtr = reinterpret_cast<Vehicle**>(carPtrPtr); // This is the pointer cast the FAQ artical talks about.

    cout << "Casting a NuclearSubmarine pointer and put it in the Car pointer." << endl;
    NuclearSubmarine  sub;
    NuclearSubmarine* subPtr = &sub;
    *vehiclePtrPtr = subPtr; // This line causeds carPtr to point to sub !
    cout << "Attempting to Start the Car." << endl;
    carPtr->startEngine();
    cout << "Attempting to open the gas cap." << endl;
    carPtr->openGasCap();  // This might call fireNuclearMissle()!
    // I have tested with the following and fired the missiles
    //      Mingw-gcc 4.7.0 on windows 7 x64
    //      Visual Studio 2010 on windows 7 x64
    //      Ubuntu 12.10 x64 GCC 4.7.2

}


#endif //missile_h
