#include "pch.h"
#pragma comment(lib, "Falcon.UI.lib")
#pragma comment(lib, "Falcon.TermEngine.lib")
#include "../Falcon.UI/MessagePipe.h"
#include "../Falcon.UI/MainWindow.h"
#include "../Falcon.UI/TerminalCanvas.h"
#include "../Falcon.TermEngine/Term.h"
#include "main.h"

using namespace Messages;
using namespace Controls;
using namespace Engine;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	Control::set_hInstance(hInstance);
	MainWindow window(L"Falcon");
	TerminalCanvas canvas;
	window.addChild(canvas, true);
	Term term(&canvas);
	window.show();

	MessagePipe::start();
	return 0;
}