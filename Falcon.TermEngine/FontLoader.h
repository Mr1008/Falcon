#pragma once
#include <dwrite.h>
#include <vector>
#include "DxCommon.h"

namespace Engine::Fonts
{
	typedef std::vector<std::wstring> MFCollection;

	class MFFontContext
	{
	public:
		MFFontContext(IDWriteFactory* pFactory);
		~MFFontContext();

		HRESULT Initialize();
		HRESULT CreateFontCollection(MFCollection& newCollection, IDWriteFontCollection** result);
		
		static const std::vector<MFCollection>& collections()
		{
			return fontCollections;
		}

	private:
		MFFontContext(MFFontContext const&);

		IDWriteFactory* factory;
		static std::vector<unsigned int> cKeys;
		static std::vector<MFCollection> fontCollections;
		HRESULT hr;

		static size_t push(MFCollection& addCollection)
		{
			size_t ret = fontCollections.size();
			fontCollections.push_back(addCollection);
			return ret;
		}
	};
}