#include "pch.h"
#include "SlaveInputInterpreter.h"
#include <sstream>
#include <algorithm>

namespace Engine
{
    using namespace std;

    SlaveInputInterpreter::SlaveInputInterpreter(TerminalBuffer* textBuffer) :
        buffer(textBuffer)
    {
    }

    void SlaveInputInterpreter::acceptInput(const wstring& input)
    {
        //buffer->inOwnedContext([&]() {
        for (auto c : input) {
            handleChar(c);
        }
        buffer->clearOldBackbuffer();
        //  });
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
            currentCommand = { L"ESC" };
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
            break;
        case L'H':
            currentCommand.stringFormat += c;
            currentCommand.numParams1 = { 0, 0 };
            setCursorPosition();
            break;
        case L'J':
            currentCommand.stringFormat += c;
            currentCommand.numParams1 = { 0 };
            eraseInDisplay();
            break;
        case L'X':
            currentCommand.stringFormat += c;
            currentCommand.numParams1 = { 0 };
            eraseCharacter();
            break;
        case L'm':
            currentCommand.stringFormat += c;
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
        currentCommand.stringFormat += c;
        if (!parseDigit(c, currentCommand.numParams1.back())) {
            switch (c)
            {
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
            case 0: {
                buffer->setAttribute(CharacterAttribute::Normal, true);
                buffer->setForegroundColor(Color(255, 255, 255));
                break; }
            case 92:
                buffer->setForegroundColor(Color(0, 255, 0));
                break;
            case 93:
                buffer->setForegroundColor(Color(255, 255, 0));
                break;
            default:
                handleUnsupportedAnsiEscapeCode();
                break;
            }
        }

        parserState = ParserState::Echo;
    }

    void SlaveInputInterpreter::setCursorPosition()
    {
        buffer->setCursorPosition(
            max(currentCommand.numParams1[1] - 1, 0),
            max(currentCommand.numParams1[0] - 1, 0)
        );
        parserState = ParserState::Echo;
    }

    void SlaveInputInterpreter::cursorForward()
    {
        buffer->cursorForward(max(currentCommand.numParams1[0], 1));
        parserState = ParserState::Echo;
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