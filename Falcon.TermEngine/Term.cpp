#include "pch.h"
#include "Term.h"

using namespace Controls;

Engine::Term::Term(Controls::TerminalCanvas *canvas)
	: canvas(canvas)
{
	canvas->listenToInput(reinterpret_cast<InputListener*>(this));
}

void Engine::Term::onMouseMoved(const POINT &pos)
{
}

void Engine::Term::onKeyPushed(wchar_t key, bool isFirstOccurence, unsigned int repeatCount)
{
	
}

void Engine::Term::onMouseButtonDown(const POINT &pos, MouseButton button)
{
	if (!canvas->hasFocus()) {
		canvas->focus();
	}
}

void Engine::Term::onMouseButtonUp(const POINT &pos, MouseButton button)
{
}
