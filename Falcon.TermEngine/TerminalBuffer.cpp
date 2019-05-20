#include "pch.h"
#include "TerminalBuffer.h"

namespace Engine
{
	using namespace std;

	TerminalBuffer::TerminalBuffer() :
		currentForegroundColor(255, 255, 255),
		currentBackgroundColor(0, 0, 0),
		currentAttributes(Normal)
	{
		newLine();
	}

	size_t TerminalBuffer::getLinesCount() const
	{
		return linesBuffer.size();
	}

	const TextLine& TerminalBuffer::getLine(size_t i)
	{
		return getEditableLine(i);
	}

	void TerminalBuffer::setAttribute(CharacterAttribute attribute, bool enabled)
	{
		if (enabled && attribute == CharacterAttribute::Normal) {
			currentAttributes = CharacterAttribute::Normal;
			return;
		}

		if (enabled) {
			currentAttributes = static_cast<CharacterAttribute>(currentAttributes | attribute);
		}
		else {
			currentAttributes = static_cast<CharacterAttribute>(currentAttributes & ~attribute);
		}
	}

	void TerminalBuffer::setForegroundColor(Color color)
	{
		currentForegroundColor = color;
	}

	void TerminalBuffer::setBackgroundColor(Color color)
	{
		currentBackgroundColor = color;
	}

	void TerminalBuffer::setCursorBlinking(bool enable)
	{
	}

	void TerminalBuffer::setCursorVisible(bool enable)
	{
	}

	void TerminalBuffer::setCursorPosition(short x, short y)
	{
	}

	void TerminalBuffer::cursorForward(short n)
	{
	}

	void TerminalBuffer::writeChar(wchar_t character)
	{
		getCurrentLine().push_back(TerminalCharacter(character, currentAttributes, currentForegroundColor, currentBackgroundColor));
		notifyAboutChange();
	}

	void TerminalBuffer::newLine()
	{
		linesBuffer.push_back(vector<TerminalCharacter>());
		cursorPosition.y++;
		cursorPosition.x = 0;
	}

	void TerminalBuffer::carriageReturn()
	{
	}

	void TerminalBuffer::eraseInDisplay(RelativePosition start, RelativePosition end)
	{
	}

	void TerminalBuffer::eraseCharacters(short n)
	{
	}

	TextLine& TerminalBuffer::getEditableLine(size_t i)
	{
		return *(linesBuffer.end() - i - 1);
	}

	TextLine& TerminalBuffer::getCurrentLine()
	{
		return getEditableLine(cursorPosition.y - 1);
	}

	const POINT& TerminalBuffer::getCursorPosition() const
	{
		return cursorPosition;
	}

	void TerminalBuffer::notifyAboutChange()
	{
		notifyListeners([this](ChangeListener * listener) {listener->onChange(this); });
	}
}
