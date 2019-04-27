#pragma once

namespace Engine
{
	class ChangeListener 
	{
	public:
		virtual void onChange(void* sender) = 0;
	};
}