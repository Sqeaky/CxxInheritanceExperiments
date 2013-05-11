// ©2013 Sqeaky
// This is distibuted under the WTFPL, see COPYING.md for details.

// This is used for a series of experiments on inheritance, polymorphism, casting and the kind of stuff that makes other programmers cringe.
// If you learn anything from this file either never apply it, apply it only in code you will maintain forever, or if you must apply it only
// in a library others with have to see it only infrequently and encapsulated away from the code they might write. Or of course use it to
// intentionally torment others. ;)

#include "vehicletree.h"

#include "diamond.h"
#include "dumbpointer.h"
#include "nestedclasses.h"
#include "missiles.h"


int main()
{
    TemplatesAndInferredCasting();
    //NestedClassSize();
    DiamondTesting();
    OpenGasCapToFireMissiles();
    return 0;
}
