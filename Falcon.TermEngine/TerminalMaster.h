#pragma once
#include "pch.h"
#include <memory>
#include <thread>
#include "TerminalWindowController.h"
#include "RendererEventsListener.h"
#include "TerminalBuffer.h"
#include "SlaveInputInterpreter.h"
#include "ChangeListener.h"

namespace Engine
{
    class TerminalMaster :
        public RendererEventsListener,
        public ChangeListener
    {
    public:
        TerminalMaster(PROCESS_INFORMATION* slave, HANDLE pipeIn, HANDLE pipeOut, HPCON con);
        void start();
        void stop();
        bool isUp() const;

        virtual void onTerminalSizeChange(const COORD& size);
        virtual void onChange(void* sender);

    private:
        std::unique_ptr<TerminalWindowController> terminalWindow;
        PROCESS_INFORMATION* slave;
        HPCON con;
        HANDLE pipeIn;
        HANDLE pipeOut;
        std::unique_ptr<std::thread> pipeListenerThread;
        std::unique_ptr<std::thread> windowThread;
        TerminalBuffer textBuffer;
        SlaveInputInterpreter inputInterpreter;

        void onSlaveInput(char* buffer, size_t bufferSize);

        friend void pipeListener(HANDLE pipe, TerminalMaster* master);
    };
}