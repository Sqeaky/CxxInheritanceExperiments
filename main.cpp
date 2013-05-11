// ©2013 Sqeaky
// This is distibuted under the WTFPL, see COPYING.md for details.

// This is used for a series of experiments on inheritance, polymorphism, casting and the kind of stuff that makes other programmers cringe.
// If you learn anything from this file either never apply it, apply it only in code you will maintain forever, or if you must apply it only
// in a library others with have to see it only infrequently and encapsulated away from the code they might write. Or of course use it to
// intentionally torment others. ;)

#include "vehicletree.h"

#include "diamond.h"
#include "dumbpointer.h"
#include "missiles.h"





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
    //error: invalid conversion from âCar*â to âChevy*â [-fpermissive]
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
