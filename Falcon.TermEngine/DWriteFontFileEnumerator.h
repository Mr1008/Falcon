#pragma once
#include <dwrite.h>
#include <vector>

namespace Engine::Fonts
{
	class DWriteFontFileEnumerator
		: public IDWriteFontFileEnumerator
	{
	public:
		DWriteFontFileEnumerator(IDWriteFactory* factory);
		~DWriteFontFileEnumerator();

		HRESULT Initialize(UINT const* collectionKey, UINT32 keySize);

		virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** ppvObject);
		virtual ULONG STDMETHODCALLTYPE AddRef();
		virtual ULONG STDMETHODCALLTYPE Release();

		virtual HRESULT STDMETHODCALLTYPE MoveNext(BOOL* hasCurrentFile);
		virtual HRESULT STDMETHODCALLTYPE GetCurrentFontFile(IDWriteFontFile** fontFile);

	private:
		ULONG refCount;

		IDWriteFactory* factory;
		IDWriteFontFile* currentFile;
		std::vector<std::wstring> filePaths;
		size_t nextIndex;
	};
}