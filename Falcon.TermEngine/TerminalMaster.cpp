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
        pipeListenerThread(nullptr)
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
        terminalWindow = make_unique<TerminalWindowController>(move(renderer), [this](wchar_t c) { onSlaveOutput(c); });
        inputInterpreter = make_unique<SlaveInputInterpreter>(&textBuffer, terminalWindow.get());
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

    void TerminalMaster::onTerminalSizeChange(const COORD& size)
    {
        ResizePseudoConsole(con, size);
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
        int convertedChars = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, buffer, bufferSize, wideBuf.get(), bufferSize);
        inputInterpreter->acceptInput(wstring(wideBuf.get(), convertedChars));
    }

    void TerminalMaster::onSlaveOutput(wchar_t c)
    {
        const size_t BUF_SIZE = 64;
        DWORD bytesWritten = 0;
        char buffer[BUF_SIZE] = {};
        int bytesToWrite = WideCharToMultiByte(CP_UTF8, 0, &c, 1, buffer, BUF_SIZE, nullptr, nullptr);
        WriteFile(pipeOut, buffer, bytesToWrite, &bytesWritten, nullptr);
    }
}