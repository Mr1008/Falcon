#pragma once
#include <d2d1.h>
#include "Control.h"

namespace Controls
{
	class TerminalRenderer
	{
	public:
		virtual void onRenderDxScene(ID2D1RenderTarget *target) = 0;
		virtual void onCreateDxResources(ID2D1RenderTarget *target) = 0;
		virtual void onReleaseDxResources() = 0;
		virtual int onResizeScene(ResizeType type, const SIZE &size) = 0;
	};
}
