#pragma once
#include "pch.h"

struct ComException
{
	HRESULT result;
	ComException(HRESULT const value) :
		result(value)
	{}
};

void HR(HRESULT const result)
{
	if (S_OK != result)
	{
		throw ComException(result);
	}
}