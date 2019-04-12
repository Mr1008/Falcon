#pragma once
#include <vector>
#include "TerminalMaster.h"

namespace Engine
{
	class Term
	{
	public:
		Term(HINSTANCE hInstance);
		void start(const std::wstring &command);
		int getReturnValue() const;

	private:
		int retVal;
		std::unique_ptr<TerminalMaster> master;

		HRESULT fillStartupInfo(STARTUPINFOEX* pStartupInfo, HPCON hPC);
		void setReturn(int retVal);
		void onSlaveIsUp(PROCESS_INFORMATION* slave, HANDLE pipeIn, HANDLE pipeOut);
		void onSlaveIsDown();
		bool isMasterUp();
	};
}

