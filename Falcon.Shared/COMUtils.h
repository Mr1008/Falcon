#pragma once
#include "pch.h"

struct ComException
{
    HRESULT result;
    ComException(HRESULT const value) :
        result(value)
    {}
};

inline void HR(HRESULT const result)
{
    if (S_OK != result)
    {
        throw ComException(result);
    }
}

template <class T>
inline void SafeRelease(T** resource)
{
    if (*resource != nullptr) {
        (*resource)->Release();
        *resource = nullptr;
    }
}

template <class T>
inline T* SafeAcquire(T* newObject)
{
    if (newObject != nullptr)
        newObject->AddRef();

    return newObject;
}