#include "pch.h"
#include "SlaveInputInterpreter.h"
#include <sstream>

namespace Engine
{
	using namespace std;

	SlaveInputInterpreter::SlaveInputInterpreter(TerminalBuffer* textBuffer) :
		buffer(textBuffer)
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
		case ParserState::CSINumParamArray:
			parser_csiNumParamArray(c);
			break;
		case ParserState::CSIQuestionMark:
			parser_csiQuestionMark(c);
			break;
		case ParserState::CSIQuestionMarkAndNumParam:
			parser_csiQuestionMarkAndNumParam(c);
			break;
		}
	}

	void SlaveInputInterpreter::parser_echoState(wchar_t c)
	{
		switch (c) {
		case L'\n':
			buffer->newLine();
			break;
		case L'\r':
			buffer->carriageReturn();
			break;
		case L'\x1b':
			parserState = ParserState::AnsiEscapeStarted;
			currentCommand = {};
			break;
		default:
			buffer->writeChar(c);
			break;
		}
	}

	void SlaveInputInterpreter::parser_ansiEscapeStarted(wchar_t c)
	{
		switch (c) {
		case L'[':
			parserState = ParserState::CSIStarted;
			break;
		default:
			handleUnsupportedAnsiEscapeCode(c);
			break;
		}
	}

	void SlaveInputInterpreter::parser_csiStarted(wchar_t c)
	{
		switch (c) {
		case L'?':
			parserState = ParserState::CSIQuestionMark;
			break;
		case L'C':
			currentCommand.numParams1 = { 0 };
			cursorForward();
			break;
		case L'H':
			currentCommand.numParams1 = { 0, 0 };
			setCursorPosition();
			break;
		case L'J':
			currentCommand.numParams1 = { 0 };
			eraseInDisplay();
			break;
		case L'X':
			currentCommand.numParams1 = { 0 };
			eraseCharacter();
			break;
		case L'm':
			currentCommand.numParams1 = { 0 };
			setGraphicsRendition(currentCommand.numParams1);
			break;
		default:
			if (iswdigit(c)) {
				parserState = ParserState::CSINumParamArray;
				currentCommand.numParams1 = { 0 };
				handleChar(c);
			}
			else {
				handleUnsupportedAnsiEscapeCode(c);
			}
			break;
		}
	}

	void SlaveInputInterpreter::parser_csiNumParamArray(wchar_t c)
	{
		if (!parseDigit(c, currentCommand.numParams1.back())) {
			switch (c) {
			case L';':
				currentCommand.numParams1.push_back(0);
				break;
			case L'C':
				cursorForward();
				break;
			case L'H':
				setCursorPosition();
				break;
			case L'J':
				eraseInDisplay();
				break;
			case L'X':
				eraseCharacter();
				break;
			case L'm':
				setGraphicsRendition(currentCommand.numParams1);
				break;
			default:
				handleUnsupportedAnsiEscapeCode(c);
				break;
			}
		}
	}

	void SlaveInputInterpreter::parser_csiQuestionMark(wchar_t c)
	{
		switch (c) {
		default:
			if (iswdigit(c)) {
				parserState = ParserState::CSIQuestionMarkAndNumParam;
				currentCommand.numParams1 = { 0 };
				handleChar(c);
			}
			else {
				handleUnsupportedAnsiEscapeCode(c);
			}
			break;
		}
	}

	void SlaveInputInterpreter::parser_csiQuestionMarkAndNumParam(wchar_t c)
	{
		if (!parseDigit(c, currentCommand.numParams1[0])) {
			switch (c)
			{
			case L'l':
			case L'h':
			{
				TNumParam param = currentCommand.numParams1[0];
				if (param == 12) {
					buffer->setCursorBlinking(c == L'h');
					parserState = ParserState::Echo;
				}
				else if (param == 25) {
					buffer->setCursorVisible(c == L'h');
					parserState = ParserState::Echo;
				}
				else {
					handleUnsupportedAnsiEscapeCode(c);
				}
				break;
			}
			default:
				handleUnsupportedAnsiEscapeCode(c);
				break;
			}
		}
	}

	void SlaveInputInterpreter::eraseInDisplay()
	{
		switch (currentCommand.numParams1[0]) {
		case 0:
			buffer->eraseInDisplay(RelativePosition::Cursor, RelativePosition::End);
			break;
		case 1:
			buffer->eraseInDisplay(RelativePosition::Begin, RelativePosition::Cursor);
			break;
		case 2:
			buffer->eraseInDisplay(RelativePosition::Begin, RelativePosition::End);
			break;
		}

		parserState = ParserState::Echo;
	}

	void SlaveInputInterpreter::eraseCharacter()
	{
		buffer->eraseCharacters(currentCommand.numParams1[0]);
		parserState = ParserState::Echo;
	}

	void SlaveInputInterpreter::setGraphicsRendition(vector<TNumParam> params)
	{
		for (TNumParam param : params) {
			switch (param)
			{
			case 0:
				buffer->setAttribute(CharacterAttribute::Normal, true);
				break;
			default:
				handleUnsupportedAnsiEscapeCode();
				return;
			}
		}

		parserState = ParserState::Echo;
	}

	void SlaveInputInterpreter::setCursorPosition()
	{
		buffer->setCursorPosition(
			max(currentCommand.numParams1[0], 1),
			max(currentCommand.numParams1[1], 1)
		);
		parserState = ParserState::Echo;
	}

	void SlaveInputInterpreter::cursorForward()
	{
		buffer->cursorForward(max(currentCommand.numParams1[0], 1));
		parserState = ParserState::Echo;
	}

	bool SlaveInputInterpreter::parseDigit(wchar_t c, TNumParam & target)
	{
		if (!iswdigit(c)) {
			return false;
		}

		target = target * 10 + (c - L'0');
		return true;
	}

	void SlaveInputInterpreter::handleUnsupportedAnsiEscapeCode(wchar_t c)
	{
		wstringstream ss;
		ss << L"Unsupported ANSI Escape Code. Falling back to echo." << endl;
		ss << L"Details:" << endl;
		ss << L"\tState: " << parserState << endl;
		ss << L"\tFailing character: " << c << endl;
		ss << L"\tNumParams1: [";
		for (TNumParam p : currentCommand.numParams1) {
			ss << p << L", ";
		}
		if (currentCommand.numParams1.size() > 0) {
			long long pos = ss.tellp();
			ss.seekp(pos - 2);
		}
		ss << L"] " << endl << endl;
		OutputDebugString(ss.str().c_str());
		parserState = ParserState::Echo;
	}
}