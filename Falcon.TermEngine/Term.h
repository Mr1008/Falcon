#pragma once
#include "TermBuffer.h"
#include "../Falcon.UI/TerminalCanvas.h"
#include "../Falcon.UI/InputListener.h"
#include "../Falcon.UI/Pointer.h"

namespace Engine
{
	class Term
		: public Controls::InputListener
	{
	public:
		Term(Controls::TerminalCanvas *canvas);

		virtual void onMouseMoved(const POINT &pos);
		virtual void onKeyPushed(wchar_t key, bool isFirstOccurence, unsigned int repeatCount);
		virtual void onMouseButtonDown(const POINT &pos, Controls::MouseButton button);
		virtual void onMouseButtonUp(const POINT &pos, Controls::MouseButton button);

	private:
		Controls::TerminalCanvas *canvas;
		TermBuffer buffer;
	};
}
