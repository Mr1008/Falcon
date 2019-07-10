#include "pch.h"
#include "SlaveInputInterpreter.h"
#include <sstream>
#include <algorithm>

namespace Engine
{
    using namespace std;
    static const wchar_t ESC = L'\x1b';
    static const wchar_t BEL = L'\x07';

    SlaveInputInterpreter::SlaveInputInterpreter(TerminalBuffer* textBuffer, TerminalWindowActions* windowActions) :
        buffer(textBuffer),
        windowActions(windowActions),
        parserState(ParserState::Echo)
    {
    }

    void SlaveInputInterpreter::acceptInput(const wstring& input)
    {
        buffer->inOwnedContext([&]() {
            for (auto c : input) {
                handleChar(c);
            }
            buffer->clearOldBackbuffer();
            });
        buffer->invalidate();
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
        case ParserState::OSCStarted:
            parser_oscStarted(c);
            break;
        case ParserState::OSCNumParam1:
            parser_oscNumParam1(c);
            break;
        case ParserState::OSCCaptureWindowTitle:
            parser_oscCaptureWindowTitle(c);
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
        case ESC:
            parserState = ParserState::AnsiEscapeStarted;
            currentCommand = { L"ESC" };
            break;
        case L'\b':
            buffer->backspace();
            break;
        default:
            buffer->writeChar(c);
            break;
        }
    }

    void SlaveInputInterpreter::parser_ansiEscapeStarted(wchar_t c)
    {
        currentCommand.stringFormat += c;
        switch (c) {
        case L'[':
            parserState = ParserState::CSIStarted;
            break;
        case L']':
            parserState = ParserState::OSCStarted;
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
            currentCommand.stringFormat += c;
            parserState = ParserState::CSIQuestionMark;
            break;
        case L'C':
            currentCommand.stringFormat += c;
            currentCommand.numParams1 = { 0 };
            cursorForward();
            parserState = ParserState::Echo;
            break;
        case L'H':
            currentCommand.stringFormat += c;
            currentCommand.numParams1 = { 0, 0 };
            setCursorPosition();
            parserState = ParserState::Echo;
            break;
        case L'J':
            currentCommand.stringFormat += c;
            currentCommand.numParams1 = { 0 };
            eraseInDisplay();
            parserState = ParserState::Echo;
            break;
        case L'X':
            currentCommand.stringFormat += c;
            currentCommand.numParams1 = { 0 };
            eraseCharacter();
            parserState = ParserState::Echo;
            break;
        case L'm':
            currentCommand.stringFormat += c;
            currentCommand.numParams1 = { 0 };
            setGraphicsRendition(currentCommand.numParams1);
            parserState = ParserState::Echo;
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
        currentCommand.stringFormat += c;
        if (!parseDigit(c, currentCommand.numParams1.back())) {
            switch (c)
            {
            case L';':
                currentCommand.numParams1.push_back(0);
                break;
            case L'C':
                cursorForward();
                parserState = ParserState::Echo;
                break;
            case L'H':
                setCursorPosition();
                parserState = ParserState::Echo;
                break;
            case L'J':
                eraseInDisplay();
                parserState = ParserState::Echo;
                break;
            case L'X':
                eraseCharacter();
                parserState = ParserState::Echo;
                break;
            case L'm':
                setGraphicsRendition(currentCommand.numParams1);
                parserState = ParserState::Echo;
                break;
            default:
                handleUnsupportedAnsiEscapeCode(c);
                break;
            }
        }
    }

    void SlaveInputInterpreter::parser_csiQuestionMark(wchar_t c)
    {
        currentCommand.stringFormat += c;
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
        currentCommand.stringFormat += c;
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

    void SlaveInputInterpreter::parser_oscStarted(wchar_t c)
    {
        switch (c) {
        default:
            if (iswdigit(c)) {
                parserState = ParserState::OSCNumParam1;
                currentCommand.numParams1 = { 0 };
                handleChar(c);
            }
            else {
                handleUnsupportedAnsiEscapeCode(c);
            }
            break;
        }
    }

    void SlaveInputInterpreter::parser_oscNumParam1(wchar_t c)
    {
        currentCommand.stringFormat += c;
        if (!parseDigit(c, currentCommand.numParams1.back())) {
            switch (c)
            {
            case L';':
                if (currentCommand.numParams1[0] == 0 || currentCommand.numParams1[0] == 2) {
                    parserState = ParserState::OSCCaptureWindowTitle;
                }
                else
                {
                    handleUnsupportedAnsiEscapeCode(c);
                }
                break;
            default:
                handleUnsupportedAnsiEscapeCode(c);
                break;
            }
        }
    }

    void SlaveInputInterpreter::parser_oscCaptureWindowTitle(wchar_t c)
    {
        switch (c)
        {
        case BEL:
            currentCommand.stringFormat += L"BEL";
            setTerminalTitle();
            parserState = ParserState::Echo;
            break;
        default:
            currentCommand.stringParam1 += c;
            currentCommand.stringFormat += c;
            break;
        }
    }

    void SlaveInputInterpreter::setTerminalTitle()
    {
        windowActions->setTitle(L"Falcon - " + currentCommand.stringParam1);
    }

    void SlaveInputInterpreter::eraseInDisplay()
    {
        switch (currentCommand.numParams1[0])
        {
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
    }

    void SlaveInputInterpreter::eraseCharacter()
    {
        buffer->eraseCharacters(currentCommand.numParams1[0]);
    }

    void SlaveInputInterpreter::setGraphicsRendition(vector<TNumParam> params)
    {
        for (TNumParam param : params) {
            switch (param)
            {
            case 0:
                buffer->setAttribute(CharacterAttribute::Normal, true);
                buffer->setForegroundColor(ColorReference::Default);
                break;
            case 30:
                buffer->setForegroundColor(ColorReference::Black);
                break;
            case 31:
                buffer->setForegroundColor(ColorReference::Red);
                break;
            case 32:
                buffer->setForegroundColor(ColorReference::Green);
                break;
            case 33:
                buffer->setForegroundColor(ColorReference::Yellow);
                break;
            case 34:
                buffer->setForegroundColor(ColorReference::Blue);
                break;
            case 35:
                buffer->setForegroundColor(ColorReference::Magenta);
                break;
            case 36:
                buffer->setForegroundColor(ColorReference::Cyan);
                break;
            case 37:
                buffer->setForegroundColor(ColorReference::White);
                break;
            case 39:
                buffer->setForegroundColor(ColorReference::Default);
                break;
            case 40:
                buffer->setBackgroundColor(ColorReference::Black);
                break;
            case 41:
                buffer->setBackgroundColor(ColorReference::Red);
                break;
            case 42:
                buffer->setBackgroundColor(ColorReference::Green);
                break;
            case 43:
                buffer->setBackgroundColor(ColorReference::Yellow);
                break;
            case 44:
                buffer->setBackgroundColor(ColorReference::Blue);
                break;
            case 45:
                buffer->setBackgroundColor(ColorReference::Magenta);
                break;
            case 46:
                buffer->setBackgroundColor(ColorReference::Cyan);
                break;
            case 47:
                buffer->setBackgroundColor(ColorReference::White);
                break;
            case 49:
                buffer->setBackgroundColor(ColorReference::Default);
                break;
            case 90:
                buffer->setForegroundColor(ColorReference::BrightBlack);
                break;
            case 91:
                buffer->setForegroundColor(ColorReference::BrightRed);
                break;
            case 92:
                buffer->setForegroundColor(ColorReference::BrightGreen);
                break;
            case 93:
                buffer->setForegroundColor(ColorReference::BrightYellow);
                break;
            case 94:
                buffer->setForegroundColor(ColorReference::BrightBlue);
                break;
            case 95:
                buffer->setForegroundColor(ColorReference::BrightMagenta);
                break;
            case 96:
                buffer->setForegroundColor(ColorReference::BrightCyan);
                break;
            case 97:
                buffer->setForegroundColor(ColorReference::BrightWhite);
                break;
            case 100:
                buffer->setBackgroundColor(ColorReference::BrightBlack);
                break;
            case 101:
                buffer->setBackgroundColor(ColorReference::BrightRed);
                break;
            case 102:
                buffer->setBackgroundColor(ColorReference::BrightGreen);
                break;
            case 103:
                buffer->setBackgroundColor(ColorReference::BrightYellow);
                break;
            case 104:
                buffer->setBackgroundColor(ColorReference::BrightBlue);
                break;
            case 105:
                buffer->setBackgroundColor(ColorReference::BrightMagenta);
                break;
            case 106:
                buffer->setBackgroundColor(ColorReference::BrightCyan);
                break;
            case 107:
                buffer->setBackgroundColor(ColorReference::BrightWhite);
                break;
            default:
                handleUnsupportedAnsiEscapeCode();
                break;
            }
        }
    }

    void SlaveInputInterpreter::setCursorPosition()
    {
        buffer->setCursorPosition(
            max(currentCommand.numParams1[1] - 1, 0),
            max(currentCommand.numParams1[0] - 1, 0)
        );
    }

    void SlaveInputInterpreter::cursorForward()
    {
        buffer->cursorForward(max(currentCommand.numParams1[0], 1));
    }

    bool SlaveInputInterpreter::parseDigit(wchar_t c, TNumParam& target)
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
        size_t nullPos = currentCommand.stringFormat.find(L'\0', 0);
        if (nullPos != wstring::npos) {
            currentCommand.stringFormat.erase(nullPos, 1);
        }
        ss << L"\tCode: " << currentCommand.stringFormat << endl;
        ss << L"\tState: " << parserState << endl;
        ss << L"\tFailing character: " << (c == L'\0' ? L"Unknown command" : wstring(c, 1)) << endl;
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
        for (auto character : currentCommand.stringFormat) {
            handleChar(character);
        }
    }
}