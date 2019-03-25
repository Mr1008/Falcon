#include "pch.h"
#include "MessagePipe.h"
#include "MainWindow.h"
#include "Terminal.h"

using namespace Messages;
using namespace Controls;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	Control::set_hInstance(hInstance);
	MainWindow window(L"Falcon");
	Terminal term;
	window.addChild(term);
	window.show();
	MessagePipe::start();
	return 0;
}