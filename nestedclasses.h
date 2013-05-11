// ©2013 Sqeaky
// This is distibuted under the WTFPL, see COPYING.md for details.

#ifndef nestedclasses_h
#define nestedclasses_h

#include "vehicletree.h"


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

#endif //nestedclasses_h
