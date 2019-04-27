#pragma once
#include <d2d1_2.h>
#include "Control.h"

namespace Controls
{
	class TerminalRenderer
	{
	public:
		virtual void onCreateDxResources(ID2D1DeviceContext *dc) = 0;
		virtual void onReleaseDxResources() = 0;
		virtual void onResizeScene(ResizeType type, const SIZE &size) = 0;
	};
}
