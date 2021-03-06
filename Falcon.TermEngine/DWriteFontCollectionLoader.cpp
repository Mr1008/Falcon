#include "pch.h"
#include "DWriteFontCollectionLoader.h"
#include "DWriteFontFileEnumerator.h"
#include "../Falcon.Shared/COMUtils.h"

namespace Engine::Fonts
{
    using namespace std;

    DWriteFontCollectionLoader::DWriteFontCollectionLoader(const vector<vector<wstring>>& fontCollections) :
        refCount(0),
        fontCollections(fontCollections)
    {
    }

    HRESULT STDMETHODCALLTYPE DWriteFontCollectionLoader::QueryInterface(REFIID iid, void** ppvObject)
    {
        if (iid == IID_IUnknown || iid == __uuidof(IDWriteFontCollectionLoader))
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

    ULONG STDMETHODCALLTYPE DWriteFontCollectionLoader::AddRef()
    {
        return InterlockedIncrement(&refCount);
    }

    ULONG STDMETHODCALLTYPE DWriteFontCollectionLoader::Release()
    {
        ULONG newCount = InterlockedDecrement(&refCount);
        if (newCount == 0)
            delete this;

        return newCount;
    }

    HRESULT STDMETHODCALLTYPE DWriteFontCollectionLoader::CreateEnumeratorFromKey(
        IDWriteFactory* factory,
        void const* collectionKey,
        UINT32 collectionKeySize,
        IDWriteFontFileEnumerator** fontFileEnumerator
    )
    {
        *fontFileEnumerator = nullptr;
        HRESULT hr = S_OK;

        if (collectionKeySize != sizeof(UINT))
            return E_INVALIDARG;

        DWriteFontFileEnumerator* enumerator = new(nothrow) DWriteFontFileEnumerator(factory, fontCollections);
        if (!enumerator)
            return E_OUTOFMEMORY;

        UINT const* mfCollectionKey = static_cast<UINT const*>(collectionKey);
        UINT32 const mfKeySize = collectionKeySize;
        hr = enumerator->Initialize(mfCollectionKey, mfKeySize);

        if (FAILED(hr))
        {
            delete enumerator;
            return hr;
        }

        *fontFileEnumerator = SafeAcquire(enumerator);
        return hr;
    }
}