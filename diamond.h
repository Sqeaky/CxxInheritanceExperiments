// ©2013 Sqeaky
// This is distibuted under the WTFPL, see COPYING.md for details.

#ifndef diamond_h
#define diamond_h

#include "vehicletree.h"


///////////////////////////////////////////////////////////////////////////
// Casting through Inheritance Diamonds

// Casting up and down inheritance can be tricky. You either have to contend with
// Multiple instances of the base classes or virtual inheritance which requires
// special effort in casting because the vtable does not store all the required data.


//////////////////////////////////////
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

//////////////////////////////////////
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
    const int RawSizeA = sizeof(DiamondA)/sizeof(int);
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
    const int RawSizeV = sizeof(DiamondV)/sizeof(int);
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
    // error: cannot convert from base âBaseVâ to derived type âDerivedV1â via virtual base âBaseVâ
    //DiamondV* BackToOriginalV = static_cast<DiamondV*>(static_cast<DerivedV1*>(BaseVPtr));
    // error: cannot convert from base âBaseVâ to derived type âDerivedV1â via virtual base âBaseVâ
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
    // error: cannot dynamic_cast âBaseVPtr->BaseV::GetMostDerived()â (of type âvoid*â) to type âclass DiamondV*â (source is not a pointer to class)
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

#endif //diamond_h
