#include "pch.h"
#include "../Falcon.TermEngine/Term.h"

using namespace Engine;

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
	Term term(hInstance);
	wchar_t *startCommand;
	size_t size;
	if (_wdupenv_s(&startCommand, &size, L"ComSpec") != 0 || startCommand == nullptr) {
		return 1;
	}
	term.start(startCommand);
	delete startCommand;
	return term.getReturnValue();
}