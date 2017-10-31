// RUN: %clang_cc1 %s -fsyntax-only -verify

#include "Singleton.h"

Singleton &Singleton::getReference() // expected-warning {{Suspicious function or static method 'getReference' having no arguments but returning pointer or reference to writable object}}
{
    static Singleton instance;
    return instance;
}

const Singleton &Singleton::getReferenceConst()
{
    static Singleton instance;
    return instance;
}

Singleton *Singleton::getPointer() // expected-warning {{Suspicious function or static method 'getPointer' having no arguments but returning pointer or reference to writable object}}
{
    static Singleton instance;
    return &instance;
}

Singleton const *Singleton::getPointerConst()
{
    static Singleton instance;
    return &instance;
}
