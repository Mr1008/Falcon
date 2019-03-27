#pragma once

namespace Exceptions
{
	class OutOfRangeException
		: public std::exception
	{
	public:
		OutOfRangeException(const std::string &message) 
			: exception(message.c_str())
		{}
	};
}
