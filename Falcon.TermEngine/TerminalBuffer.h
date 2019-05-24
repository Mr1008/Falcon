#pragma once
#include "pch.h"
#include <vector>
#include <mutex>
#include "../Falcon.Shared/Publisher.h"
#include "ChangeListener.h"
#include "Colors.h"

namespace Engine
{
	enum RelativePosition {
		Begin,
		End,
		Cursor
	};

	enum CharacterAttribute
	{
		Normal = 0,
		Bold = 1 << 0,
		Faint = 1 << 1,
		Italic = 1 << 2,
		Underline = 1 << 3,
		Blink = 1 << 4,
		Invisible = 1 << 5,
		CrossedOut = 1 << 6,
		DoublyUnderlined = 1 << 7
	};

	struct TerminalCharacter
	{
		TerminalCharacter(wchar_t character, CharacterAttribute attributes, Color foreground, Color background) :
			character(character),
			attributes(attributes),
			foregroundColor(foreground),
			backgroundColor(background)
		{}

		wchar_t character;
		CharacterAttribute attributes;
		Color foregroundColor;
		Color backgroundColor;
	};

	struct VisibleRange {
		size_t topLine;
		size_t bottomLine;
	};

	typedef std::vector<TerminalCharacter> TextLine;

	class TerminalBuffer :
		public Shared::Publisher<ChangeListener>
	{
	public:
		TerminalBuffer();

		size_t getLinesCount() const;
		const TextLine& getLine(size_t i);
		const POINT& getCursorPosition() const;
		void invalidate();
		void inOwnedContext(std::function<void()> fn);
		void clearOldBackbuffer();
		bool hasChanges();
		void clearChanged();
		void setVisibleRange(size_t topLine, size_t bottomLine);

		void setAttribute(CharacterAttribute attribute, bool enabled);
		void setForegroundColor(ColorReference color); // TODO
		void setForegroundColor(Color color);
		void setBackgroundColor(ColorReference color); // TODO
		void setBackgroundColor(Color color);
		void setCursorBlinking(bool enable);
		void setCursorVisible(bool enable);
		void setCursorPosition(short x, short y);
		void cursorForward(short n);
		void writeChar(wchar_t character);
		void newLine();
		void carriageReturn();
		void eraseInDisplay(RelativePosition start, RelativePosition end);
		void eraseCharacters(short n);

	private:
		const size_t BACKBUFFER_SIZE = 600;
		Color currentForegroundColor;
		Color currentBackgroundColor;
		CharacterAttribute currentAttributes;
		std::vector<TextLine> linesBuffer;
		POINT cursorPosition;
		std::mutex freezeMutex;
		bool hasChangesFlag;
		VisibleRange visibleRange;

		TextLine& getEditableLine(size_t i);
		TextLine& getCurrentLine();

		void notifyAboutChange();
	};
}
