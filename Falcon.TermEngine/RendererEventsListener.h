#pragma once
#include "pch.h"

namespace Engine
{
	class RendererEventsListener
	{
	public:
		virtual void onTerminalSizeChange(const COORD& size) = 0;
	};
}
