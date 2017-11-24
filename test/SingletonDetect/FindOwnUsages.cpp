// RUN: %clang_cc1 %s -fsyntax-only -verify -plugin-arg-glutamat -level=evil

#include "Singleton.h"

void Singleton::nonStaticMethod()
{
    int &intRef = getIntReference();

    Singleton &self = getReference(); // expected-warning {{Getting pointer or reference to singleton 'Singleton' in its non-static method does not make sense}}
}
