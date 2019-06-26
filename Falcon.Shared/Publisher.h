#pragma once
#include <functional>
#include <vector>
#include <algorithm>

namespace Shared
{
    template <class TListener>
    class Publisher
    {
    public:
        void registerListener(TListener* listener);
        void unregisterListener(TListener* listener);

    protected:
        void notifyListeners(std::function<void(TListener*)> fn);

    private:
        std::vector<TListener*> listeners;
    };

    template<class TListener>
    void Publisher<TListener>::registerListener(TListener* listener)
    {
        listeners.push_back(listener);
    }

    template<class TListener>
    void Publisher<TListener>::unregisterListener(TListener* listener)
    {
        void(std::remove(listeners.begin(), listeners.end(), listener));
    }

    template<class TListener>
    void Publisher<TListener>::notifyListeners(std::function<void(TListener*)> fn)
    {
        for (auto l : listeners) {
            fn(l);
        }
    }
}
