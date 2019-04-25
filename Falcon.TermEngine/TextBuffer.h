#pragma once
#include <vector>

namespace Engine
{
	class TextBuffer
	{
	public:
		TextBuffer();

		void acceptInput(const std::wstring &input);
		void setWidth(size_t width);
		size_t getLinesCount() const;
		const std::wstring& getLine(size_t i) const;
		
	private:
		enum ParserState {
			Echo,
			AnsiEscapeStarted,
			CSIStarted,
			OSIStarted
		};

		const size_t BACKBUFFER_SIZE = 20;
		size_t currentWidth;

		std::vector<std::wstring> linesBuffer;
		ParserState parserState;

		void handleChar(wchar_t c);
		void newLine();

		void parser_echoState(wchar_t c);
		void parser_ansiEscapeStarted(wchar_t c);
		void parser_csiStarted(wchar_t c);
	};

	struct AnsiEscapeSequence 
	{

	};
}
