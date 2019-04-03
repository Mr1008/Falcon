#pragma once

namespace Engine
{
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
}