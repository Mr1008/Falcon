#pragma once
#include "pch.h"
#include <memory>
#include <thread>
#include "TerminalWindow.h"

namespace Engine
{
	class TerminalMaster
	{
	public:
		TerminalMaster(PROCESS_INFORMATION* slave, HANDLE pipeIn, HANDLE pipeOut);
		void start();
		void stop();

	private:
		std::unique_ptr<TerminalWindow> terminalWindow;
		PROCESS_INFORMATION* slave;
		HANDLE pipeIn;
		HANDLE pipeOut;
		std::unique_ptr<std::thread> pipeListenerThread;
		std::unique_ptr<std::thread> windowThread;

		void onSlaveInput(char* buffer, size_t bufferSize);

		friend void pipeListener(HANDLE pipe, TerminalMaster* master);
	};
}