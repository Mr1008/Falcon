#pragma once
#include "pch.h"
#include <memory>
#include <thread>
#include "TerminalWindowController.h"
#include "TerminalWindowListener.h"
#include "TextBuffer.h"

namespace Engine
{
	class TerminalMaster :
		public TerminalWindowListener
	{
	public:
		TerminalMaster(PROCESS_INFORMATION* slave, HANDLE pipeIn, HANDLE pipeOut, HPCON con);
		void start();
		void stop();
		bool isUp() const;

		virtual void onWindowResize(const COORD& size);

	private:
		std::unique_ptr<TerminalWindowController> terminalWindow;
		PROCESS_INFORMATION* slave;
		HPCON con;
		HANDLE pipeIn;
		HANDLE pipeOut;
		std::unique_ptr<std::thread> pipeListenerThread;
		std::unique_ptr<std::thread> windowThread;
		TextBuffer textBuffer;

		void onSlaveInput(char* buffer, size_t bufferSize);

		friend void pipeListener(HANDLE pipe, TerminalMaster* master);
	};
}