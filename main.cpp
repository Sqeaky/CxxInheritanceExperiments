// ©2013 Sqeaky
// This is distibuted under the WTFPL, see COPYING.md for details.

// This is used for a series of experiments on inheritance, polymorphism, casting and the kind of stuff that makes other programmers cringe.
// If you learn anything from this file either never apply it, apply it only in code you will maintain forever, or if you must apply it only
// in a library others with have to see it only infrequently and encapsulated away from the code they might write. Or of course use it to
// intentionally torment others. ;)

#include <iostream>
#include <cstring>

using namespace std;

///////////////////////////////////////////////////////////////////////////
// Really I just want opening the gas cap to fire the missiles

// This example was pulled directly from http://www.parashift.com/c++-faq/derivedptrptr-to-baseptrptr.html
//
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
    { cout << "Start a car." << endl; }

void Car::openGasCap()
    { cout << "Open a Car's gas cap." << endl; }

void NuclearSubmarine::startEngine()
    { cout << "Start a NuclearSubmarine engine." << endl; }

void NuclearSubmarine::fireNuclearMissle()
{
    cout << "Fire the MISSILES!!!!" << endl;
    // I have tested with the following and fired the missiles
    //      Mingw-gcc 4.7.0 on windows 7 x64
    //      Visual Studio 2010 on windows 7 x64
    //      Ubuntu 12.10 x64 GCC 4.7.2
}

void OpenGasCapToFireMissiles()
{
    cout << "Creating Car pointer pointer." << endl;
    Car   car;
    Car*  carPtr = &car;
    Car** carPtrPtr = &carPtr;

    //error: invalid conversion from ‘Car**’ to ‘Vehicle**’ [-fpermissive]
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
}


///////////////////////////////////////////////////////////////////////////
// Casting through Inheritance Diamonds

// Normal inheritance Diamond

// On most compilers an int has sizeof(int) == 4. If that is the case it would seem intuitive that
// a class with 4 int members would have sizeof(class) == 4*sizeof(int) == 16 (assuming no alignment issues) or that a
// class which inherited to get 4 int members would have that size. The result of a normal inheritance diamond leaves
// two copies of the base most class in the resulting class one for each middle derived member. So the resulting size
// is sizeof(int)*5 or 20 in common cases.
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

// Virtual inheritance causes a more derived type to call any virtually inheritted types constructors once (in this
// example anyway). This means that there should only be 4 ints and now the most derived type should have
// sizeof(class) == sizeof(int)*4 but it is bigger. The extra space is pointers in the vTable. Nothing guarantees a
// vTable be used, but it is the simplest and most performant way to implement the standard.
class BaseV
{
    public:
        int BaseValue;
        virtual void* GetMostDerived() // returning a void* is very poor form, who knows what to do with it.
            { return reinterpret_cast<void*>(this); }
};

class DerivedV1 : public virtual BaseV  // Note the virtual inheritance
{
    public:
        int Derived1Value;
        virtual void* GetMostDerived() // This is a hack that I will try to use to downcast past virtual inheritance.
            { return reinterpret_cast<void*>(this); }
};

class DerivedV2 : public virtual BaseV
{
    public:
        int Derived2Value;
        virtual void* GetMostDerived()
            { return reinterpret_cast<void*>(this); }
};

class DiamondV : public DerivedV1, public DerivedV2
{
    public:
        int DiamondValue;
        virtual void* GetMostDerived()
            { return reinterpret_cast<void*>(this); }
};

// For descriptions of this BS
//   for one compiler implementation of inheritance diamonds: http://deepakgiri123.blogspot.com/2011/05/c-up-casting-down-casting-in.html
//   for a concise explanation of up and downcasting: http://www.linuxtopia.org/online_books/programming_books/thinking_in_c++/Chapter15_028.html
void DiamondTesting()
{
    // Since everything in C++ is about memory and offsets here are some sizes, so that inferences might be made more easily.
    cout << "int size: " << sizeof(int) << endl
         << "int* size: " << sizeof(int*) << endl << endl
         << "Normal Diamond inheritance sizes " << endl
         << "BaseA:     " << sizeof(BaseA) << endl
         << "DerivedA1: " << sizeof(DerivedA1) << endl
         << "DerivedA2: " << sizeof(DerivedA2) << endl
         << "DiamondA:  " << sizeof(DiamondA) << endl << endl
         << "Virtual Diamond inheritance sizes " << endl
         << "BaseV:     " << sizeof(BaseV) << endl
         << "DerivedV1: " << sizeof(DerivedV1) << endl
         << "DerivedV2: " << sizeof(DerivedV2) << endl
         << "DiamondV:  " << sizeof(DiamondV) << endl
         << endl;

    DiamondA* TestA = new DiamondA; // If you new it delete it, ctrl+f for zzzzz
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

    // This very clearly shows the layout in memory of normal inheritance.
    int RawSizeA = sizeof(DiamondA)/sizeof(int);
    int DiamondARaw[RawSizeA];
    memcpy(DiamondARaw, TestA, sizeof(DiamondA));
    cout << "Normal Inheritance Raw Memory Layout as integers" << endl
         << "Offset\tValue" << endl;
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


    // At the time I wrote this most compilers seem to use 4 byte pointer and a 4 byte int, on my Linux x64 machine
    // an 8 byte pointer is provided, the results is that this inspection routine spread the pointer across two
    // entries in the array. When I ran this all the pointers had one 4 byte group with the pointer and the rest set
    // to zeroes, but there will come a day when that is not true. If you must cast a pointer to an number type
    // use an intptr_t as it is guaranteed to be large enough to store a pointer. This shows that on the
    // implemtations there where always at least two pointers include to manage the virtual inheritance.
    int RawSizeV = sizeof(DiamondV)/sizeof(int);
    int DiamondVRaw[RawSizeV];
    memcpy(DiamondVRaw, TestV, sizeof(DiamondV));
    cout << "Virtual Inheritance Raw Memory Layout as integers" << endl
         << "Offset\tValue" <<endl;
    for(int c = 0; c < RawSizeV; c++)
    {
        cout << c << "\t" << DiamondVRaw[c] << endl;
    }
    cout << "0 as pointer              \t" << (int*)(DiamondARaw) <<endl;
    cout << ((sizeof(int)+sizeof(int*))/sizeof(int)) << " as First VPtr(Maybe) \t" << (int*)(DiamondARaw)+((sizeof(int)+sizeof(int*))/sizeof(int)) <<endl;
    cout << endl;

    // Upcasts(casting towards bases) should just work because every derived type IsA base type.
    //BaseA* BaseAPtr = TestA;        //error: 'BaseA' is an ambiguous base of 'DiamondA'
    // Because there are multiple instances of the basemost class in this diamond, the desired one must be
    // specified with a cast to less derived type that inherits only one copy of the base type.
    BaseA* BaseCastA1Ptr = dynamic_cast<DerivedA1*>(TestA);
    BaseA* BaseCastA2Ptr = dynamic_cast<DerivedA2*>(TestA);
    DerivedA1* BaseDA1Ptr = TestA;
    DerivedA2* BaseDA2Ptr = TestA;

    BaseV* BaseVPtr = TestV;
    DerivedV1* BaseDV1Ptr = TestV;
    DerivedV2* BaseDV2Ptr = TestV;

    cout << "RawPointer for normal Diamond: " << endl
         << "Ptr:       Address \tAs number \tOffset" << endl
         << "Original:  " << TestA << "\t" << (long long)(TestA) << "\t" << reinterpret_cast<char*>(TestA)-reinterpret_cast<char*>(TestA) << endl
         << "BaseA(1):  " << BaseCastA1Ptr << "\t" << (long long)(BaseCastA1Ptr) << "\t" << reinterpret_cast<char*>(BaseCastA1Ptr)-reinterpret_cast<char*>(TestA) << endl
         << "BaseA(2):  " << BaseCastA2Ptr << "\t" << (long long)(BaseCastA2Ptr) << "\t" << reinterpret_cast<char*>(BaseCastA2Ptr)-reinterpret_cast<char*>(TestA) << endl
         << "DerivedA1: " << BaseDA1Ptr << "\t" << (long long)(BaseDA1Ptr) << "\t" << reinterpret_cast<char*>(BaseDA1Ptr)-reinterpret_cast<char*>(TestA) << endl
         << "DerivedA2: " << BaseDA2Ptr << "\t" << (long long)(BaseDA2Ptr) << "\t" << reinterpret_cast<char*>(BaseDA2Ptr)-reinterpret_cast<char*>(TestA) << endl
         << "DiamondA:  " << TestA << "\t" << (long long)(TestA) << "\t" << reinterpret_cast<char*>(TestA)-reinterpret_cast<char*>(TestA) << endl
         << endl;

    cout << "RawPointer for virtual Diamond: " << endl
         << "Ptr:       Address \tOffset" << endl
         << "Original:  " << TestV << "\t" << (long long)(TestV) << "\t" << reinterpret_cast<char*>(TestV)-reinterpret_cast<char*>(TestV) << endl
         << "BaseV   :  " << BaseVPtr << "\t" << (long long)(BaseVPtr) << "\t" << reinterpret_cast<char*>(BaseVPtr)-reinterpret_cast<char*>(TestV) << endl
         << "DerivedV1: " << BaseDV1Ptr << "\t" << (long long)(BaseDV1Ptr) << "\t" << reinterpret_cast<char*>(BaseDV1Ptr)-reinterpret_cast<char*>(TestV) << endl
         << "DerivedV2: " << BaseDV2Ptr << "\t" << (long long)(BaseDV2Ptr) << "\t" << reinterpret_cast<char*>(BaseDV2Ptr)-reinterpret_cast<char*>(TestV) << endl
         << "DiamondV:  " << TestV << "\t" << (long long)(TestV) << "\t" << reinterpret_cast<char*>(TestV)-reinterpret_cast<char*>(TestV) << endl
         << endl;


    // Downcasts, because every base might not be a derived, you need to request a specific derived class
    DiamondA* BackToOriginalA1 = static_cast<DiamondA*>(static_cast<DerivedA1*>(BaseCastA1Ptr));
    DiamondA* BackToOriginalA2 = static_cast<DiamondA*>(static_cast<DerivedA2*>(BaseCastA2Ptr));
    DerivedA1* BackToDerivedA1 = static_cast<DerivedA1*>(BaseCastA1Ptr);
    DerivedA2* BackToDerivedA2 = static_cast<DerivedA2*>(BaseCastA2Ptr);

    // Without the multiple casts here are the errors GCC gave me.
    // error: cannot convert from base ‘BaseV’ to derived type ‘DerivedV1’ via virtual base ‘BaseV’
    //DiamondV* BackToOriginalV = static_cast<DiamondV*>(static_cast<DerivedV1*>(BaseVPtr));
    // error: cannot convert from base ‘BaseV’ to derived type ‘DerivedV1’ via virtual base ‘BaseV’
    //DerivedV1* BacktoDerivedV = static_cast<DerivedV1*>(BaseVPtr);

    // Downcasts across the virtual inheritance lines are complicated by the extra indirection that
    // forcing only one of the most base type to exist cause. Here are the address

    cout << "Address of pointers of virtually inherited types and their addresses:" << endl
         << "BaseV:     \t" << BaseVPtr << " \tGetMostDerived: \t" << BaseVPtr->GetMostDerived() << endl
         << "DerivedV1: \t" << BaseDV1Ptr << " \tGetMostDerived: \t" << BaseDV1Ptr->GetMostDerived() << endl
         << "DerivedV1: \t" << BaseDV2Ptr << " \tGetMostDerived: \t" << BaseDV2Ptr->GetMostDerived() << endl
         << "DiamondV:  \t" << TestV << " \tGetMostDerived: \t" << TestV->GetMostDerived() << endl
         << endl;

    DiamondV* StaticDiamondV = static_cast<DiamondV*>(BaseVPtr->GetMostDerived());
    DiamondV* ReinterpretDiamondV = reinterpret_cast<DiamondV*>(BaseVPtr->GetMostDerived());
    // error: cannot dynamic_cast ‘BaseVPtr->BaseV::GetMostDerived()’ (of type ‘void*’) to type ‘class DiamondV*’ (source is not a pointer to class)
    //DiamondV* DynamicDiamondV = dynamic_cast<DiamondV*>(BaseVPtr->GetMostDerived()); //

    cout << "Address of Pointers cast from BaseVPtr->GetMostDerived(): " << endl
         << "Original:       \t" << TestV << endl
         << "Static cast:    \t" << StaticDiamondV << endl
         << "Reinterpretted: \t" << ReinterpretDiamondV << endl
         << "Dynamic cast:   \tCompilation error. Cannot cast from (void*)." << endl
         << endl;

    delete TestV; // zzzzz
    delete TestA;




}

///////////////////////////////////////////////////////////////////////////

/* These experiments are not interesting yet
class NDefault
{
    public:
        int* Target;
};

class NNestedClass
{
    public:
        int* Target;

        class HasGoMethod
        {
            public:
                static void Go()
                    { cout << "Called NNestedClass::HasGoMethod::go();"; }
        };
};

class NNestedFunctor
{
    public:
        int* Target;

        class IsFunctor
        {
            public:
                void operator() ()
                    { cout << "Called NNestedFunctor::IsFunctor::go();"; }
        };

        IsFunctor Go;
};



void NestedClassSize()
{
    cout    << "Size of Test classes" << endl
            << "sizeof(NDefault):       " << sizeof(NDefault) << endl
            << "sizeof(NNestedClass):   " << sizeof(NNestedClass) << endl
            << "sizeof(NNestedFunctor): " << sizeof(NNestedFunctor) << endl
            << endl;

    NNestedClass::HasGoMethod::Go();




    cout << endl;
}
*/

///////////////////////////////////////////////////////////////////////////
// Templates and casting to derived types.

// In this test the car branch of the vehicle hiearchy will serve as the targets for some very basic smart pointer proxies.
class Chevy : public Car {
    public:
        virtual void startEngine();
        virtual void openGasCap();
};

void Chevy::startEngine()
    { cout << "Start a Chevy." << endl; }

void Chevy::openGasCap()
    { cout << "Open a Chevy's gas cap." << endl; }

// Here Is the default for a the dumbed down smart pointer(hereafter dumb pointer)
template <typename A, typename B>
class TemplateImpliesType
{
    private:
        B* ValuePointer;

    public:
        TemplateImpliesType(B* Init)
            : ValuePointer(Init)
        {}

        A* Get()
        {
            cout << "Returning a B* as an A*." << endl;
            return dynamic_cast<A*>(ValuePointer);
        }
};

// If the instantiation type matches the return type, this will get used instead.
template <typename A>
class TemplateImpliesType <A,A>
{
    private:
        A* ValuePointer;

    public:
        TemplateImpliesType(A* Init)
            : ValuePointer(Init)
        {}

        A* Get()
        {
            cout << "Returning an A*." << endl;
            return ValuePointer;
        }

};

// This one uses a default template parameter to allow a dumb-pointer to be implemented with the support for casting
// between types but not complicate matters by requiring a separate class.
template <typename A, typename B=A>
class TemplateImpliesType2
{
    private:
        B* ValuePointer;

    public:
        TemplateImpliesType2(B* Init)
            : ValuePointer(Init)
        {}

        A* Get()
        {
            cout << "Returning a B* or an A* as an A*." << endl;
            return dynamic_cast<A*>(ValuePointer);
        }
};


void TemplatesAndInferredCasting()
{
    cout << "Creating some vehicles." << endl;
    Car TargetA;
    Chevy TargetB;
    cout << endl;

    cout << "Stuffing vehicles into class templates that may obscure their types." << endl;
    TemplateImpliesType<Car,Car> ATest(&TargetA);
    TemplateImpliesType<Car,Chevy> BTest(&TargetB);
    ATest.Get()->startEngine();
    BTest.Get()->startEngine();
    cout << endl;

    cout << "Stuffing vehicles into class templates that may obscure their types and gets the inheritance hiearchy backwards. But it would not compile" << endl;
    TemplateImpliesType<Chevy,Car> AwTest(&TargetB);
    //error: invalid conversion from ‘Car*’ to ‘Chevy*’ [-fpermissive]
    //TemplateImpliesType<Chevy,Chevy> BwTest(&TargetA);
    AwTest.Get()->startEngine();
    //BwTest.Get()->startEngine();
    cout << endl;

    cout << "Stuffing vehicle into type that can handle multiple type and doesn't care." << endl;
    TemplateImpliesType2<Car> A2Test(&TargetA);
    TemplateImpliesType2<Car> B2Test(&TargetB);
    A2Test.Get()->openGasCap();
    B2Test.Get()->openGasCap();
    cout << endl;
}


int main()
{
    TemplatesAndInferredCasting();
    //NestedClassSize();
    DiamondTesting();
    OpenGasCapToFireMissiles();
    return 0;
}
















