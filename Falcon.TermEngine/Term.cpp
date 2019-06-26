#include "pch.h"
#include "Term.h"
#include "../Falcon.UI/Control.h"
#pragma comment(lib, "Falcon.UI.lib")

namespace Engine
{
    using namespace Controls;
    using namespace std;

    Term::Term(HINSTANCE hInstance) :
        master(nullptr),
        retVal(1)
    {
        Control::set_hInstance(hInstance);
    }

    HRESULT Term::fillStartupInfo(STARTUPINFOEX* startupInfo, HPCON con)
    {
        HRESULT hr = E_UNEXPECTED;

        if (startupInfo)
        {
            SIZE_T attrListSize = 0;

            startupInfo->StartupInfo.cb = sizeof(STARTUPINFOEX);
            InitializeProcThreadAttributeList(nullptr, 1, 0, &attrListSize);
            startupInfo->lpAttributeList = reinterpret_cast<LPPROC_THREAD_ATTRIBUTE_LIST>(malloc(attrListSize));

            if (startupInfo->lpAttributeList
                && InitializeProcThreadAttributeList(startupInfo->lpAttributeList, 1, 0, &attrListSize))
            {
                hr = UpdateProcThreadAttribute(
                    startupInfo->lpAttributeList,
                    0,
                    PROC_THREAD_ATTRIBUTE_PSEUDOCONSOLE,
                    con,
                    sizeof(HPCON),
                    nullptr,
                    nullptr) ? S_OK : HRESULT_FROM_WIN32(GetLastError());
            }
            else
            {
                hr = HRESULT_FROM_WIN32(GetLastError());
            }
        }
        return hr;
    }

    void Term::start(const wstring& command)
    {
        HRESULT hr = E_UNEXPECTED;
        HANDLE pipePtyIn = INVALID_HANDLE_VALUE;
        HANDLE pipePtyOut = INVALID_HANDLE_VALUE;
        HANDLE pipeIn = INVALID_HANDLE_VALUE;
        HANDLE pipeOut = INVALID_HANDLE_VALUE;
        HPCON con;

        if (!CreatePipe(&pipePtyIn, &pipeOut, nullptr, 0) ||
            !CreatePipe(&pipeIn, &pipePtyOut, nullptr, 0))
        {
            setReturn(1);
        }
        else
        {
            hr = CreatePseudoConsole({ 100, 100 }, pipePtyIn, pipePtyOut, 0, &con);
            if (pipePtyOut != INVALID_HANDLE_VALUE) {
                CloseHandle(pipePtyOut);
            }
            if (pipePtyIn != INVALID_HANDLE_VALUE) {
                CloseHandle(pipePtyIn);
            }

            STARTUPINFOEX startupInfo{};
            if (S_OK == fillStartupInfo(&startupInfo, con))
            {
                auto cmd = unique_ptr<wchar_t>(new wchar_t[command.size() + 1]);
                wcscpy_s(cmd.get(), command.size() + 1, command.c_str());
                PROCESS_INFORMATION piClient{};
                hr = CreateProcess(
                    nullptr,
                    cmd.get(),
                    nullptr,
                    nullptr,
                    false,
                    EXTENDED_STARTUPINFO_PRESENT,
                    nullptr,
                    nullptr,
                    &startupInfo.StartupInfo,
                    &piClient) ? S_OK : GetLastError();

                if (S_OK == hr)
                {
                    onSlaveIsUp(&piClient, pipeIn, pipeOut, con);
                    while (WaitForSingleObject(piClient.hThread, 1000) == WAIT_TIMEOUT)
                    {
                        if (!isMasterUp()) {
                            TerminateProcess(piClient.hProcess, 1);
                        }
                    }
                }
                else
                {
                    setReturn(2);
                }

                DWORD exitCode;
                if (!GetExitCodeProcess(piClient.hProcess, &exitCode)) {
                    exitCode = -1;
                }
                setReturn(exitCode);

                CloseHandle(piClient.hThread);
                CloseHandle(piClient.hProcess);
                if (startupInfo.lpAttributeList != nullptr) {
                    DeleteProcThreadAttributeList(startupInfo.lpAttributeList);
                    free(startupInfo.lpAttributeList);
                }
            }

            ClosePseudoConsole(con);
            if (pipeOut != INVALID_HANDLE_VALUE) {
                CloseHandle(pipeOut);
            }
            if (pipeIn != INVALID_HANDLE_VALUE) {
                CloseHandle(pipeIn);
            }
            onSlaveIsDown();
        }
    }

    int Term::getReturnValue() const
    {
        return retVal;
    }

    void Term::setReturn(int value)
    {
        retVal = value;
    }

    void Term::onSlaveIsUp(PROCESS_INFORMATION* slave, HANDLE pipeIn, HANDLE pipeOut, HPCON con)
    {
        master = make_unique<TerminalMaster>(slave, pipeIn, pipeOut, con);
        master->start();
    }

    void Term::onSlaveIsDown()
    {
        if (master != nullptr) {
            master->stop();
        }
    }

    bool Term::isMasterUp()
    {
        if (master == nullptr) {
            return false;
        }

        return master->isUp();
    }
}
