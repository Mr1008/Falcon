#pragma once
#include "ResizeType.h"
#include "InputListener.h"

namespace Controls
{
    class ControlEventListener :
        public InputListener
    {
    public:
        virtual void onSizeChanged(ResizeType type, const SIZE& size) = 0;
    };
}
