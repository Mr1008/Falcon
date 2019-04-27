#include "pch.h"
#include "TextBuffer.h"

namespace Engine
{
	using namespace std;

	TextBuffer::TextBuffer() :
		currentWidth(0)
	{
		newLine();
	}

	void TextBuffer::setWidth(size_t width)
	{
		currentWidth = width;
	}

	size_t TextBuffer::getLinesCount() const
	{
		return linesBuffer.size();
	}

	const std::wstring& TextBuffer::getLine(size_t i) const
	{
		return *(linesBuffer.end() - 1 - i);
	}

	void TextBuffer::newLine()
	{
		linesBuffer.push_back(L"");
	}

	void TextBuffer::notifyAboutChange()
	{
		notifyListeners([this](ChangeListener * listener) {listener->onChange(this); });
	}
}