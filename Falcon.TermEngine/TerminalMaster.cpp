#include "pch.h"
#include "TerminalMaster.h"

namespace Engine
{
	using namespace std;

	TerminalMaster::TerminalMaster(PROCESS_INFORMATION* slave, HANDLE pipeIn, HANDLE pipeOut) :
		slave(slave),
		pipeIn(pipeIn),
		pipeOut(pipeOut),
		pipeListenerThread(nullptr)
	{
	}

	void pipeListener(HANDLE pipe, TerminalMaster* master)
	{
		const size_t BUFFER_SIZE = 512;
		char buffer[BUFFER_SIZE]{};
		DWORD bytesRead = 0;
		bool isPipeUp = false;
		do
		{
			isPipeUp = ReadFile(pipe, buffer, BUFFER_SIZE, &bytesRead, nullptr);
			master->onSlaveInput(buffer, bytesRead);

		} while (isPipeUp && bytesRead >= 0);
	}

	void TerminalMaster::start()
	{
		windowThread = make_unique<thread>(
			[this]() {
				terminalWindow = make_unique<TerminalWindow>();
				terminalWindow->show();
			});

		pipeListenerThread = make_unique<thread>(pipeListener, pipeIn, this);
	}

	void TerminalMaster::stop()
	{
		terminalWindow->close();
		windowThread->join();
		pipeListenerThread->join();
	}

	void TerminalMaster::onSlaveInput(char* buffer, size_t bufferSize)
	{

	}
}