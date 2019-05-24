#include "pch.h"
#include "TerminalBuffer.h"

namespace Engine
{
	using namespace std;

	TerminalBuffer::TerminalBuffer() :
		currentForegroundColor(255, 255, 255),
		currentBackgroundColor(0, 0, 0),
		currentAttributes(Normal),
		cursorPosition({ 0, 0 })
	{
		linesBuffer.push_back(vector<TerminalCharacter>());
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
		// cursorPosition = { x,y };
	}

	void TerminalBuffer::cursorForward(short n)
	{
	}

	void TerminalBuffer::writeChar(wchar_t character)
	{
		TextLine& line = getCurrentLine();
		int x = cursorPosition.x;
		size_t size = line.size();
		TerminalCharacter newCharacter(character, currentAttributes, currentForegroundColor, currentBackgroundColor);
		if (x > size) {
			line.insert(line.end(), x - line.size(), TerminalCharacter(L' ', currentAttributes, currentForegroundColor, currentBackgroundColor));
		}
		if (x < size) {
			line[x] = move(newCharacter);
		}
		else {
			line.push_back(move(newCharacter));
		}
		cursorPosition.x++;
	}

	void TerminalBuffer::newLine()
	{
		linesBuffer.push_back(vector<TerminalCharacter>());
		cursorPosition.y++;
	}

	void TerminalBuffer::carriageReturn()
	{
		cursorPosition.x = 0;
	}

	void TerminalBuffer::eraseInDisplay(RelativePosition start, RelativePosition end)
	{
		switch (start) {
		case RelativePosition::Begin:
			break;
		case RelativePosition::Cursor:
			break;
		default:
			return; // Do nothing. Invalid command.
		}

		switch (end) {
		case RelativePosition::Cursor:
			break;
		case RelativePosition::End:
			break;
		default:
			return; // Do nothing. Invalid command.
		}


	}

	void TerminalBuffer::eraseCharacters(short n)
	{
	}

	TextLine& TerminalBuffer::getEditableLine(size_t i)
	{
		return *(linesBuffer.begin() + i);
	}

	TextLine& TerminalBuffer::getCurrentLine()
	{
		return getEditableLine(cursorPosition.y);
	}

	const POINT& TerminalBuffer::getCursorPosition() const
	{
		return cursorPosition;
	}

	void TerminalBuffer::invalidate()
	{
		notifyAboutChange();
		hasChangesFlag = true;
	}

	void TerminalBuffer::inOwnedContext(std::function<void()> fn)
	{
		freezeMutex.lock();
		fn();
		freezeMutex.unlock();
	}

	void TerminalBuffer::notifyAboutChange()
	{
		notifyListeners([this](ChangeListener* listener) {listener->onChange(this); });
	}

	void TerminalBuffer::clearOldBackbuffer()
	{
		size_t count = getLinesCount();
		if (count <= BACKBUFFER_SIZE) {
			return;
		}

		size_t toRemove = count - BACKBUFFER_SIZE;
		linesBuffer.erase(linesBuffer.begin(), linesBuffer.begin() + toRemove);
		cursorPosition.y -= toRemove;
	}

	bool TerminalBuffer::hasChanges()
	{
		return hasChangesFlag;
	}

	void TerminalBuffer::clearChanged()
	{
		hasChangesFlag = false;
	}

	void TerminalBuffer::setVisibleRange(size_t topLine, size_t bottomLine)
	{
		visibleRange = { topLine, bottomLine };
	}
}
