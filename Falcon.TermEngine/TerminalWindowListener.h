#pragma once

namespace Engine
{
	class TerminalWindowListener
	{
	public:
		virtual void onWindowResize(const COORD& size) = 0;
	};
}
