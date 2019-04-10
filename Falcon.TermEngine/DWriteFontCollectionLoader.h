#include <dwrite.h>
#include <vector>

namespace Engine::Fonts
{
	class DWriteFontCollectionLoader
		: public IDWriteFontCollectionLoader
	{
	public:
		DWriteFontCollectionLoader(const std::vector<std::vector<std::wstring>> &fontCollections);

		virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** ppvObject);
		virtual ULONG STDMETHODCALLTYPE AddRef();
		virtual ULONG STDMETHODCALLTYPE Release();
		virtual HRESULT STDMETHODCALLTYPE CreateEnumeratorFromKey(
			IDWriteFactory* factory,
			void const* collectionKey,
			UINT32 collectionKeySize,
			IDWriteFontFileEnumerator** fontFileEnumerator
		);

	private:
		ULONG refCount;
		const std::vector<std::vector<std::wstring>>& fontCollections;
	};
}