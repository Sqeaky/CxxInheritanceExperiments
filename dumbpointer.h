// ©2013 Sqeaky
// This is distibuted under the WTFPL, see COPYING.md for details.

#ifndef dumbpointer_h
#define dumbpointer_h

#include "vehicletree.h"

///////////////////////////////////////////////////////////////////////////
// Templates and casting to derived types.


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

        template <typename X, typename Y>   // This has to be a class to detect types at compile time.
        class GetImpl
        {
            public:
                static X* Get(Y* Results)
                {
                    cout << "Returning a B* as an A*." << endl;
                    return dynamic_cast<A*>(Results);
                }

        };

        template <typename X>
        class GetImpl <X,X>
        {
            public:
                static X* Get(X* Results)
                {
                    cout << "Returning an A* as an A*." << endl;
                    return Results;
                }

        };

    public:
        TemplateImpliesType2(B* Init)
            : ValuePointer(Init)
        {}

        A* Get()
        {
            return GetImpl<A,B>::Get(ValuePointer);
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
    //error: invalid conversion from Car* to Chevy* [-fpermissive]
    //TemplateImpliesType<Chevy,Chevy> BwTest(&TargetA);
    AwTest.Get()->startEngine();
    //BwTest.Get()->startEngine();
    cout << endl;

    cout << "Stuffing vehicle into type that can handle multiple type and doesn't care." << endl;
    TemplateImpliesType2<Car> A2Test(&TargetA);
    TemplateImpliesType2<Car,Chevy> B2Test(&TargetB);
    A2Test.Get()->openGasCap();
    B2Test.Get()->openGasCap();
    cout << endl;
}
#endif //dumbpointer_h
