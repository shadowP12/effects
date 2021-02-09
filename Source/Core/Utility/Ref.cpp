#include "Core/Utility/Ref.h"

Ref::Ref() {
    mRefCount = 1;
}

Ref::~Ref() {
}

void Ref::incref() {
    ++mRefCount;
}

void Ref::decref() {
    if ((--mRefCount) <= 0) {
        delete this;
    }
}

unsigned int Ref::getRefCount() const {
    return mRefCount;
}