#pragma once
#include <vector>
#include "../Falcon.Shared/Publisher.h"
#include "ChangeListener.h"

namespace Engine
{
	class TextBuffer :
		public Shared::Publisher<ChangeListener>
	{
	public:
		TextBuffer();

		void setWidth(size_t width);
		size_t getLinesCount() const;
		const std::wstring& getLine(size_t i) const;
		
	private:
		const size_t BACKBUFFER_SIZE = 20;
		size_t currentWidth;

		std::vector<std::wstring> linesBuffer;
		void newLine();
		void notifyAboutChange();
	};
}
