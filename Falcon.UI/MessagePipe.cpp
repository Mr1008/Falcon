#include "pch.h"
#include "MessagePipe.h"

namespace Messages
{
	void MessagePipe::start()
	{
		MSG msg = {};
		while (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}