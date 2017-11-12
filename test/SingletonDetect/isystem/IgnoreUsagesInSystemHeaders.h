#include "Singleton.h"

Singleton *ignored() // expected-warning{{Suspicious function or static method 'ignored' having no arguments but returning pointer or reference to writable object}}
{
    return Singleton::getPointer(); // expected-warning {{Usage of suspicious function or static method 'Singleton::getPointer' having no arguments but returning pointer or reference to writable object}}
}
