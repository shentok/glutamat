// RUN: %clang_cc1 %s -fsyntax-only -verify

class Singleton
{
public:
    static int &getReference();

    static int const &getReferenceConst();

    static int *getPointer();

    static int const *getPointerConst();
};

int &Singleton::getReference() // expected-warning {{Suspicious function or static method 'getReference' having no arguments but returning pointer or reference to writable object}}
{
    static int result = 0;
    return result;
}

const int &Singleton::getReferenceConst()
{
    static int result = 0;
    return result;
}

int *Singleton::getPointer() // expected-warning {{Suspicious function or static method 'getPointer' having no arguments but returning pointer or reference to writable object}}
{
    static int result = 0;
    return &result;
}

int const *Singleton::getPointerConst()
{
    static int result = 0;
    return &result;
}

#if 0
class Pointer {
    int *m_pointer;

    void setPointer(int *const pointer);

    int getValue();

    int &getReference();

    int const &getReferenceConst();

    int *getPointer();

    int const *getPointerConst();
};


void Pointer::setPointer(int *const pointer)
{
    m_pointer = pointer;
}

int Pointer::getValue() // expected-warning {{function 'getValue' could be declared as const}}
{
    return *m_pointer;
}

int &Pointer::getReference()
{
    return *m_pointer;
}

const int &Pointer::getReferenceConst() // expected-warning {{function 'getReferenceConst' could be declared as const}}
{
    return *m_pointer;
}

int *Pointer::getPointer()
{
    return m_pointer;
}

const int *Pointer::getPointerConst() // expected-warning {{function 'getPointerConst' could be declared as const}}
{
    return m_pointer;
}

class ConstPointer {
    const int * m_pointer;

public:
    void setPointer(int *foo);
};


void ConstPointer::setPointer(int *pointer)
{
    m_pointer = pointer;
}


struct ComplexType
{
    void foo();
    void foo() const;
};

class Foo
{
public:
    void useValue();

private:
    ComplexType m_value;
};


void Foo::useValue() // expected-warning {{function 'useValue' could be declared as const}}
{
    m_value.foo();
}
#endif
