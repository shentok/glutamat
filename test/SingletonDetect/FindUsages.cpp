// RUN: %clang_cc1 %s -fsyntax-only -verify

#include "Singleton.h"
#include "IgnoreUsagesOutsideMainFile.h"

int main(int, char **)
{
    const Singleton &constRef = Singleton::getReferenceConst();
    Singleton &ref = Singleton::getReference(); // expected-warning {{Usage of suspicious function or static method 'Singleton::getReference' having no arguments but returning pointer or reference to writable object}}

    const Singleton *constPointer = Singleton::getPointerConst();
    Singleton *pointer = Singleton::getPointer(); // expected-warning {{Usage of suspicious function or static method 'Singleton::getPointer' having no arguments but returning pointer or reference to writable object}}
}
