#pragma once
#include <dwrite.h>
#include <vector>
#include "DWriteFontCollectionLoader.h"

namespace Engine::Fonts
{
	class DWriteFontLoader
	{
	public:
		DWriteFontLoader(IDWriteFactory* factory);
		~DWriteFontLoader();

		HRESULT CreateFontCollection(std::vector<std::wstring>& newCollection, IDWriteFontCollection** result);

	private:
		DWriteFontLoader(DWriteFontLoader const&);

		IDWriteFactory* factory;
		DWriteFontCollectionLoader* loader;
		std::vector<unsigned int> keysCollection;
		std::vector<std::vector<std::wstring>> fontCollections;
		HRESULT hr;
	};
}