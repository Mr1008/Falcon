#include "pch.h"
#include "FontLoader.h"
#include "DWriteFontCollectionLoader.h"

namespace Engine::Fonts
{
	MFFontContext::MFFontContext(IDWriteFactory* pFactory) : hr(S_FALSE), factory(pFactory)
	{
	}

	MFFontContext::~MFFontContext()
	{
		factory->UnregisterFontCollectionLoader(DWriteFontCollectionLoader::GetLoader());
	}

	HRESULT MFFontContext::Initialize()
	{
		if (hr == S_FALSE)
		{
			if (!DWriteFontCollectionLoader::IsLoaderInitialized())
				return E_FAIL;

			hr = factory->RegisterFontCollectionLoader(DWriteFontCollectionLoader::GetLoader());
		}
		return hr;
	}

	HRESULT MFFontContext::CreateFontCollection(MFCollection& newCollection, IDWriteFontCollection** result)
	{
		*result = NULL;
		HRESULT hr = S_OK;

		size_t collectionKey = push(newCollection);
		cKeys.push_back(collectionKey);
		const void* fontCollectionKey = &cKeys.back();
		UINT32 keySize = sizeof(collectionKey);

		hr = Initialize();
		if (FAILED(hr))
			return hr;

		hr = factory->CreateCustomFontCollection(
			DWriteFontCollectionLoader::GetLoader(),
			fontCollectionKey,
			keySize,
			result
		);

		return hr;
	}

	std::vector<unsigned int> MFFontContext::cKeys = std::vector<unsigned int>(0);

	std::vector<MFCollection> MFFontContext::fontCollections = std::vector<MFCollection>(0);
}