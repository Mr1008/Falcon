#pragma once
#include "pch.h"
#include "Pointer.h"

namespace Controls
{
	class InputListener
	{
	public:
		virtual void onKeyPushed(wchar_t key, bool isFirstOccurence, unsigned int repeatCount) = 0;
		virtual void onMouseMoved(const POINT &pos) = 0;
		virtual void onMouseButtonDown(const POINT &pos, MouseButton button) = 0;
		virtual void onMouseButtonUp(const POINT &pos, MouseButton button) = 0;
	};
}
