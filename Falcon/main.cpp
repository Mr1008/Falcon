#include "pch.h"
#include "MessagePipe.h"
#include "MainWindow.h"

using namespace Messages;
using namespace Views;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	ViewBase::set_hInstance(hInstance);
	MainWindow window(L"Falcon");
	window.show();

	MessagePipe::start();
	return 0;
}