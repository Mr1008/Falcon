#pragma once
#include "TerminalBuffer.h"
#include <vector>

namespace Engine
{
    class SlaveInputInterpreter
    {
    public:
        SlaveInputInterpreter(TerminalBuffer* textBuffer);
        void acceptInput(const std::wstring& input);

    private:
        enum ParserState {
            Echo,
            AnsiEscapeStarted,
            CSIStarted,
            CSINumParamArray,
            CSIQuestionMark,
            CSIQuestionMarkAndNumParam
        };

        typedef short TNumParam;

        struct AnsiEscapeCommand {
            std::wstring stringFormat;
            std::vector<TNumParam> numParams1;
        };

        TerminalBuffer* buffer;
        ParserState parserState;
        AnsiEscapeCommand currentCommand;

        void handleChar(wchar_t c);
        void parser_echoState(wchar_t c);
        void parser_ansiEscapeStarted(wchar_t c);
        void parser_csiStarted(wchar_t c);
        void parser_csiNumParamArray(wchar_t c);
        void parser_csiQuestionMark(wchar_t c);
        void parser_csiQuestionMarkAndNumParam(wchar_t c);

        void eraseInDisplay();
        void eraseCharacter();
        void setGraphicsRendition(std::vector<TNumParam> params);
        void setCursorPosition();
        void cursorForward();

        bool parseDigit(wchar_t c, TNumParam& target);
        void handleUnsupportedAnsiEscapeCode(wchar_t c = L'\0');
    };
}