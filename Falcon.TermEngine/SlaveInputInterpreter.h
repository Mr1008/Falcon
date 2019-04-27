#pragma once
#include "TextBuffer.h"

namespace Engine
{
	class SlaveInputInterpreter
	{
	public:
		SlaveInputInterpreter(TextBuffer *textBuffer);
		void acceptInput(const std::wstring& input);
	
	private:
		enum ParserState {
			Echo,
			AnsiEscapeStarted,
			CSIStarted,
			OSIStarted
		};

		TextBuffer* textBuffer;
		ParserState parserState;

		void handleChar(wchar_t c);
		void parser_echoState(wchar_t c);
		void parser_ansiEscapeStarted(wchar_t c);
		void parser_csiStarted(wchar_t c);
	};
}