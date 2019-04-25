#include "pch.h"
#include "TextBuffer.h"

namespace Engine
{
	using namespace std;

	TextBuffer::TextBuffer() :
		currentWidth(0),
		parserState(ParserState::Echo)
	{
		newLine();
	}

	void TextBuffer::acceptInput(const wstring& input)
	{
		for (auto c : input) {
			handleChar(c);
		}
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

	void TextBuffer::handleChar(wchar_t c)
	{
		switch (parserState) {
		case ParserState::Echo:
			parser_echoState(c);
			break;
		case ParserState::AnsiEscapeStarted:
			parser_ansiEscapeStarted(c);
			break;
		case ParserState::CSIStarted:
			parser_csiStarted(c);
			break;
		}
	}

	void TextBuffer::newLine()
	{
		linesBuffer.push_back(L"");
	}

	void TextBuffer::parser_echoState(wchar_t c)
	{
		switch (c) {
		case L'\n':
			newLine();
			break;
		case L'\x1b':
			parserState = ParserState::AnsiEscapeStarted;
			break;
		default:
			wstring& line = linesBuffer.back();
			line.push_back(c);
			break;
		}
	}

	void TextBuffer::parser_ansiEscapeStarted(wchar_t c)
	{
		switch (c) {
		case L'A':
			break;
		case L'B':
			break;
		case L'C':
			break;
		case L'D':
			break;
		case L'M':
			break;
		case L'7':
			break;
		case L'8':
			break;
		case L'[':
			parserState = ParserState::CSIStarted;
			break;
		case L']':
			parserState = ParserState::OSIStarted;
			break;
		}
	}

	void TextBuffer::parser_csiStarted(wchar_t c)
	{
		
	}
}