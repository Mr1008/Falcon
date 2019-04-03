#include "pch.h"
#include "DWriteFontFileEnumerator.h"
#include "DxCommon.h"
#include "FontLoader.h"

namespace Engine::Fonts
{
	DWriteFontFileEnumerator::DWriteFontFileEnumerator(IDWriteFactory* factory)
		: refCount(0), factory(SafeAcquire(factory)), currentFile(), nextIndex(0)
	{
	}

	DWriteFontFileEnumerator::~DWriteFontFileEnumerator()
	{
		SafeRelease(&currentFile);
		SafeRelease(&factory);
	}

	HRESULT DWriteFontFileEnumerator::Initialize(UINT const* collectionKey, UINT32 keySize)
	{
		try
		{
			UINT cPos = *collectionKey;
			for (auto it = MFFontContext::collections()[cPos].begin(); it != MFFontContext::collections()[cPos].end(); ++it)
			{
				filePaths.push_back(it->c_str());
			}
		}
		catch (...)
		{
			return E_FAIL;
		}
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE DWriteFontFileEnumerator::QueryInterface(REFIID iid, void** ppvObject)
	{
		if (iid == IID_IUnknown || iid == __uuidof(IDWriteFontFileEnumerator))
		{
			*ppvObject = this;
			AddRef();
			return S_OK;
		}
		else
		{
			*ppvObject = NULL;
			return E_NOINTERFACE;
		}
	}

	ULONG STDMETHODCALLTYPE DWriteFontFileEnumerator::AddRef()
	{
		return InterlockedIncrement(&refCount);
	}

	ULONG STDMETHODCALLTYPE DWriteFontFileEnumerator::Release()
	{
		ULONG newCount = InterlockedDecrement(&refCount);
		if (newCount == 0)
			delete this;

		return newCount;
	}

	HRESULT STDMETHODCALLTYPE DWriteFontFileEnumerator::MoveNext(BOOL * hasCurrentFile)
	{
		HRESULT hr = S_OK;

		*hasCurrentFile = FALSE;
		SafeRelease(&currentFile);

		if (nextIndex < filePaths.size())
		{
			hr = factory->CreateFontFileReference(
				filePaths[nextIndex].c_str(),
				NULL,
				&currentFile
			);

			if (SUCCEEDED(hr))
			{
				*hasCurrentFile = TRUE;

				++nextIndex;
			}
		}

		return hr;
	}

	HRESULT STDMETHODCALLTYPE DWriteFontFileEnumerator::GetCurrentFontFile(IDWriteFontFile * *fontFile)
	{
		*fontFile = SafeAcquire(currentFile);
		return (currentFile != NULL) ? S_OK : E_FAIL;
	}
}
