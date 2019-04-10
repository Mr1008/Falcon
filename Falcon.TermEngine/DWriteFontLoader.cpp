#include "pch.h"
#include "DWriteFontLoader.h"

namespace Engine::Fonts
{
	DWriteFontLoader::DWriteFontLoader(IDWriteFactory* factory) : 
		hr(S_FALSE), 
		factory(factory), 
		keysCollection(),
		fontCollections(),
		loader(new DWriteFontCollectionLoader(fontCollections))
	{
		hr = factory->RegisterFontCollectionLoader(loader);
	}

	DWriteFontLoader::~DWriteFontLoader()
	{
		factory->UnregisterFontCollectionLoader(loader);
	}

	HRESULT DWriteFontLoader::CreateFontCollection(std::vector<std::wstring>& newCollection, IDWriteFontCollection** result)
	{
		*result = NULL;
		HRESULT hr = S_OK;

		size_t collectionKey = fontCollections.size();
		fontCollections.push_back(newCollection);
		keysCollection.push_back(collectionKey);
		const void* fontCollectionKey = &keysCollection.back();
		UINT32 keySize = sizeof(collectionKey);

		hr = factory->CreateCustomFontCollection(loader, fontCollectionKey, keySize, result);
		return hr;
	}
}