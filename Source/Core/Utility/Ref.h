#pragma once
class Ref
{
public:
    Ref();
    virtual ~Ref();
    void incref();
    void decref();
    unsigned int getRefCount() const;
private:
    unsigned int mRefCount;
};