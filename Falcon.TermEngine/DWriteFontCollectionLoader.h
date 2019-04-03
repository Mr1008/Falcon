#include <dwrite.h>

namespace Engine::Fonts
{
	class DWriteFontCollectionLoader
		: public IDWriteFontCollectionLoader
	{
	public:
		DWriteFontCollectionLoader();

		virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** ppvObject);
		virtual ULONG STDMETHODCALLTYPE AddRef();
		virtual ULONG STDMETHODCALLTYPE Release();
		virtual HRESULT STDMETHODCALLTYPE CreateEnumeratorFromKey(
			IDWriteFactory* factory,
			void const* collectionKey,
			UINT32 collectionKeySize,
			IDWriteFontFileEnumerator** fontFileEnumerator
		);

		static IDWriteFontCollectionLoader* GetLoader();
		static bool IsLoaderInitialized();

	private:
		ULONG refCount;
		static IDWriteFontCollectionLoader* instance;
	};
}