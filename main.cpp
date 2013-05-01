#include <iostream>
#include <cstring>

using namespace std;

///////////////////////////////////////////////////////////////////////////
// really I just want opening the gas cap to fire the missiles
class Vehicle {
    public:
        virtual ~Vehicle() { }
        virtual void startEngine() = 0;
};

class Car : public Vehicle {
    public:
        virtual void startEngine();
        virtual void openGasCap();
};

class NuclearSubmarine : public Vehicle {
    public:
        virtual void startEngine();
        virtual void fireNuclearMissle();
};

void Car::startEngine()
{
    cout << "Start a car." << endl;
}

void Car::openGasCap()
{
    cout << "Open a Car's gas cap." << endl;
}

void NuclearSubmarine::startEngine()
{
    cout << "Start a NuclearSubmarine engine." << endl;
}

void NuclearSubmarine::fireNuclearMissle()
{
    cout << "Fire the MISSILES!!!!" << endl;
    //Currently I have tested with the following and fired the missiles
    //  Mingw-gcc 4.7.0 on windows 7 x64
    //  Visual Studio 2010 on windows 7 x64
}

void OpenGasCapToFireMissiles()
{
    cout << "Creating Car pointer pointer." << endl;
    Car   car;
    Car*  carPtr = &car;
    Car** carPtrPtr = &carPtr;
    //Vehicle** vehiclePtrPtr = carPtrPtr;  // This is an error in C++
    cout << "Creating car pointer." << endl;
    cout << "Reinterpret casting a NuclearSubmarine pointer pointer and put it in the Car pointer pointer." << endl;
    Vehicle** vehiclePtrPtr = reinterpret_cast<Vehicle**>(carPtrPtr);
    NuclearSubmarine  sub;
    NuclearSubmarine* subPtr = &sub;
    *vehiclePtrPtr = subPtr;
    // This last line would have caused carPtr to point to sub !
    cout << "Attempting to Start the Vehicle." << endl;
    carPtr->startEngine();
    cout << "Attempting to open the gas cap." << endl;
    carPtr->openGasCap();  // This might call fireNuclearMissle()!
}


///////////////////////////////////////////////////////////////////////////
// Casting through Inheritance Diamonds

// Normal inheritance Diamond
class BaseA
{
    public:
        int BaseValue;
};

class DerivedA1 : public BaseA
{
    public:
        int Derived1Value;
};

class DerivedA2 : public BaseA
{
    public:
        int Derived2Value;
};

class DiamondA : public DerivedA1, public DerivedA2
{
    public:
        int DiamondValue;
};

// Virtual inheritance Diamond
class BaseV
{
    public:
        int BaseValue;
};

class DerivedV1 : public virtual BaseV
{
    public:
        int Derived1Value;
};

class DerivedV2 : public virtual BaseV
{
    public:
        int Derived2Value;
};

class DiamondV : public DerivedV1, public DerivedV2
{
    public:
        int DiamondValue;
};

void DiamondTesting()
{
    cout << "int size: " << sizeof(int) << endl << endl
         << "Normal Diamond inheritance sizes " << endl
         << "BaseA:     " << sizeof(BaseA) << endl
         << "DerivedA1: " << sizeof(DerivedA1) << endl
         << "DerivedA2: " << sizeof(DerivedA2) << endl
         << "DiamondA:  " << sizeof(DiamondA) << endl << endl
         << "Virtual Diamond inheritance sizes " << endl
         << "BaseV:     " << sizeof(BaseV) << endl
         << "DerivedV1: " << sizeof(DerivedV1) << endl
         << "DerivedV2: " << sizeof(DerivedV2) << endl
         << "DiamondV:  " << sizeof(DiamondV) << endl << endl
        ;

    DiamondA* TestA = new DiamondA;
    TestA->DerivedA1::BaseValue = 0;
    TestA->DerivedA2::BaseValue = 1;
    TestA->Derived1Value = 2;
    TestA->Derived2Value = 3;
    TestA->DiamondValue = 4;
    cout << "Putting values into DiamondV: " << TestA->DerivedA1::BaseValue << " "
         << TestA->DerivedA2::BaseValue << " "
         << TestA->Derived1Value << " "
         << TestA->Derived2Value << " "
         << TestA->DiamondValue << " " << endl;

    int RawSizeA = sizeof(DiamondA)/sizeof(int);
    int DiamondARaw[RawSizeA];
    memcpy(DiamondARaw, TestA, sizeof(DiamondA));
    cout << "Normal Inheritance Raw Memory Layout as integers" << endl
         << "Offset\tValue" <<endl;
    for(int c = 0; c < RawSizeA; c++)
    {
        cout << c << "\t" << DiamondARaw[c] << endl;
    }
    cout << endl;


    DiamondV* TestV = new DiamondV;
    TestV->BaseValue = 0;
    TestV->Derived1Value = 1;
    TestV->Derived2Value = 2;
    TestV->DiamondValue = 3;
    cout << "Putting values into DiamondV: " << TestV->BaseValue << " "
         << TestV->Derived1Value << " "
         << TestV->Derived2Value << " "
         << TestV->DiamondValue << " " << endl;

    int RawSizeV = sizeof(DiamondV)/sizeof(int);
    int DiamondVRaw[RawSizeV];
    memcpy(DiamondVRaw, TestV, sizeof(DiamondV));
    cout << "Virtual Inheritance Raw Memory Layout as integers" << endl
         << "Offset\tValue" <<endl;
    for(int c = 0; c < RawSizeV; c++)
    {
        cout << c << "\t" << DiamondVRaw[c] << endl;
    }
    cout << endl;

    //Downcasts should just work
    //BaseA* BaseAPtr = TestA;        //error: 'BaseA' is an ambiguous base of 'DiamondA'
    BaseA* BaseCastA1Ptr = dynamic_cast<DerivedA1*>(TestA);
    BaseA* BaseCastA2Ptr = dynamic_cast<DerivedA2*>(TestA);
    DerivedA1* BaseDA1Ptr = TestA;
    DerivedA2* BaseDA2Ptr = TestA;

    BaseV* BaseVPtr = TestV;
    DerivedV1* BaseDV1Ptr = TestV;
    DerivedV2* BaseDV2Ptr = TestV;

    cout << "RawPointer for normal Diamond: " << endl
         << "Ptr:       Address \tOffset" << endl
         << "Original:  " << TestA << "\t" << reinterpret_cast<char*>(TestA)-reinterpret_cast<char*>(TestA) << endl
         << "BaseA(1):  " << BaseCastA1Ptr << "\t" << reinterpret_cast<char*>(BaseCastA1Ptr)-reinterpret_cast<char*>(TestA) << endl
         << "BaseA(2):  " << BaseCastA2Ptr << "\t" << reinterpret_cast<char*>(BaseCastA2Ptr)-reinterpret_cast<char*>(TestA) << endl
         << "DerivedA1: " << BaseDA1Ptr << "\t" << reinterpret_cast<char*>(BaseDA1Ptr)-reinterpret_cast<char*>(TestA) << endl
         << "DerivedA2: " << BaseDA2Ptr << "\t" << reinterpret_cast<char*>(BaseDA2Ptr)-reinterpret_cast<char*>(TestA) << endl
         << "DiamondA:  " << TestA << "\t" << reinterpret_cast<char*>(TestA)-reinterpret_cast<char*>(TestA) << endl << endl
        ;

    cout << "RawPointer for virtual Diamond: " << endl
         << "Ptr:       Address \tOffset" << endl
         << "Original:  " << TestV << "\t" << reinterpret_cast<char*>(TestV)-reinterpret_cast<char*>(TestV) << endl
         << "BaseV   :  " << BaseVPtr << "\t" << reinterpret_cast<char*>(BaseVPtr)-reinterpret_cast<char*>(TestV) << endl
         << "DerivedV1: " << BaseDV1Ptr << "\t" << reinterpret_cast<char*>(BaseDV1Ptr)-reinterpret_cast<char*>(TestV) << endl
         << "DerivedV2: " << BaseDV2Ptr << "\t" << reinterpret_cast<char*>(BaseDV2Ptr)-reinterpret_cast<char*>(TestV) << endl
         << "DiamondV:  " << TestV << "\t" << reinterpret_cast<char*>(TestV)-reinterpret_cast<char*>(TestV) << endl << endl
        ;


    DiamondA* BackToOriginalA1 = static_cast<DiamondA*>(static_cast<DerivedA1*>(BaseCastA1Ptr));
    DiamondA* BackToOriginalA2 = static_cast<DiamondA*>(static_cast<DerivedA2*>(BaseCastA2Ptr));
    DerivedA1* BackToDerivedA1 = static_cast<DerivedA1*>(BaseCastA1Ptr);
    DerivedA2* BackToDerivedA2 = static_cast<DerivedA2*>(BaseCastA2Ptr);

    // error: cannot convert from base ‘BaseV’ to derived type ‘DerivedV1’ via virtual base ‘BaseV’
    //DiamondV* BackToOriginalV = static_cast<DiamondV*>(static_cast<DerivedV1*>(BaseVPtr));
    //error: cannot convert from base ‘BaseV’ to derived type ‘DerivedV1’ via virtual base ‘BaseV’
    //DerivedV1* BacktoDerivedV = static_cast<DerivedV1*>(BaseVPtr);


    delete TestV;
    delete TestA;
}


///////////////////////////////////////////////////////////////////////////


int main()
{
      DiamondTesting();
      OpenGasCapToFireMissiles();
      return 0;
}
















