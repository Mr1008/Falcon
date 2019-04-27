#include "pch.h"
#include "TerminalMaster.h"
#include "DxTerminalRenderer.h"

namespace Engine
{
	using namespace std;

	TerminalMaster::TerminalMaster(PROCESS_INFORMATION* slave, HANDLE pipeIn, HANDLE pipeOut, HPCON con) :
		con(con),
		slave(slave),
		pipeIn(pipeIn),
		pipeOut(pipeOut),
		pipeListenerThread(nullptr),
		inputInterpreter(&textBuffer)
	{
		textBuffer.registerListener(this);
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
		auto renderer = make_unique<DxTerminalRenderer>(&textBuffer);
		renderer->registerListener(this);
		terminalWindow = make_unique<TerminalWindowController>(move(renderer));
		windowThread = make_unique<thread>(
			[this]() {
				terminalWindow->show();
			});

		pipeListenerThread = make_unique<thread>(pipeListener, pipeIn, this);
	}

	void TerminalMaster::stop()
	{
		terminalWindow->close();
		windowThread->join();
		windowThread = nullptr;
		pipeListenerThread->join();
		pipeListenerThread = nullptr;
	}

	bool TerminalMaster::isUp() const
	{
		if (terminalWindow == nullptr) {
			return false;
		}

		return terminalWindow->isUp();
	}

	void TerminalMaster::onTerminalSizeChange(const COORD & size)
	{
		ResizePseudoConsole(con, size);
		textBuffer.setWidth(size.Y);
	}

	void TerminalMaster::onChange(void* sender)
	{
		if (sender == reinterpret_cast<void*>(&textBuffer) && terminalWindow->isUpAndRendererReady()) {
			terminalWindow->render();
		}
	}

	void TerminalMaster::onSlaveInput(char* buffer, size_t bufferSize)
	{
		auto wideBuf = make_unique<wchar_t[]>(bufferSize);
		MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, buffer, bufferSize, wideBuf.get(), bufferSize);
		inputInterpreter.acceptInput(wstring(wideBuf.get(), bufferSize));
	}
}