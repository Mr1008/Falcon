#include "pch.h"
#include "SlaveInputInterpreter.h"

namespace Engine
{
	using namespace std;

	SlaveInputInterpreter::SlaveInputInterpreter(TextBuffer* textBuffer) :
		textBuffer(textBuffer)
	{
	}

	void SlaveInputInterpreter::acceptInput(const wstring& input)
	{
		for (auto c : input) {
			handleChar(c);
		}
	}

	void SlaveInputInterpreter::handleChar(wchar_t c)
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

	void SlaveInputInterpreter::parser_echoState(wchar_t c)
	{
		switch (c) {
		case L'\n':
			break;
		case L'\x1b':
			parserState = ParserState::AnsiEscapeStarted;
			break;
		default:
			break;
		}
	}

	void SlaveInputInterpreter::parser_ansiEscapeStarted(wchar_t c)
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

	void SlaveInputInterpreter::parser_csiStarted(wchar_t c)
	{

	}
}