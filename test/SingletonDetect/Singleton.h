#pragma once

class Singleton
{
public:
    static Singleton &getReference();

    static Singleton const &getReferenceConst();

    static Singleton *getPointer();

    static Singleton const *getPointerConst();

    static int &getIntReference();

    void nonStaticMethod();
};
