#pragma once
#include <vector>
#include "Exceptions.h"

namespace Engine
{
	class TermBuffer
	{
	public:
		TermBuffer()
		{
			lineBuffer.push_back(L"TEST");
		}

		const std::wstring& getLine(size_t line)
		{
			if (line > lineBuffer.size())
				throw Exceptions::OutOfRangeException("No such line in a buffer.");
			return lineBuffer[line];
		}

	private:
		std::vector<std::wstring> lineBuffer;
	};
}
