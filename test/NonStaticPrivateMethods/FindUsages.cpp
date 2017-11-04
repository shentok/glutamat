// RUN: %clang_cc1 %s -fsyntax-only -verify

class Singleton
{
public:
    static int &getReference();

    static int const &getReferenceConst();

    static int *getPointer();

    static int const *getPointerConst();
};

int main(int, char **)
{
    const int &constRef = Singleton::getReferenceConst();
    int ref = Singleton::getReference(); // expected-warning {{Usage of suspicious function or static method 'Singleton::getReference' having no arguments but returning pointer or reference to writable object}}

    const int *constPointer = Singleton::getPointerConst();
    int *pointer = Singleton::getPointer(); // expected-warning {{Usage of suspicious function or static method 'Singleton::getPointer' having no arguments but returning pointer or reference to writable object}}
}
