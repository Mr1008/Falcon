#pragma once
namespace Engine
{
	class TextBuffer
	{
	public:
		void setLineWidth(size_t widthInCharacters);

	private:
		size_t widthInCharacters;
	};
}
