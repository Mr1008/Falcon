#pragma once
#include "TerminalBuffer.h"
#include "TerminalWindowActions.h"
#include <vector>

namespace Engine
{
    class SlaveInputInterpreter
    {
    public:
        SlaveInputInterpreter(TerminalBuffer* textBuffer, TerminalWindowActions* windowActions);
        void acceptInput(const std::wstring& input);

    private:
        enum ParserState {
            Echo,
            AnsiEscapeStarted,

            //CSI
            CSIStarted,
            CSINumParamArray,
            CSIQuestionMark,
            CSIQuestionMarkAndNumParam,

            //OSC
            OSCStarted,
            OSCNumParam1,
            OSCCaptureWindowTitle
        };

        typedef short TNumParam;

        struct AnsiEscapeCommand {
            std::wstring stringFormat;
            std::vector<TNumParam> numParams1;
            std::wstring stringParam1;
        };

        TerminalWindowActions* windowActions;
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
        void parser_oscStarted(wchar_t c);
        void parser_oscNumParam1(wchar_t c);
        void parser_oscCaptureWindowTitle(wchar_t c);

        void eraseInDisplay();
        void eraseCharacter();
        void setGraphicsRendition(std::vector<TNumParam> params);
        void setCursorPosition();
        void cursorForward();
        void setTerminalTitle();

        bool parseDigit(wchar_t c, TNumParam& target);
        void handleUnsupportedAnsiEscapeCode(wchar_t c = L'\0');
    };
}