#include "WiimoteHidWin32.h"
#include <process.h>

namespace WiimoteDriver
{
	WiimoteHidWin32::WiimoteHidWin32(WiimoteCallback* callback, std::wstring devicePath) :
			WiimoteHid(callback),
			m_devicePath(devicePath),
			m_read(INVALID_HANDLE_VALUE),
			m_write(INVALID_HANDLE_VALUE),
			m_thread(INVALID_HANDLE_VALUE)
	{
		// Initialize the CriticalSection
		InitializeCriticalSection(&m_criticalSection);
		try
		{
			// Create handle for reading
			if ((m_read = CreateFile(devicePath.c_str(), GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL)) == INVALID_HANDLE_VALUE) throw GetLastError();
			// Create handle for writing
			if ((m_write = CreateFile(devicePath.c_str(), GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL)) == INVALID_HANDLE_VALUE) throw GetLastError();
			//Get the Capabilities of the device
			PHIDP_PREPARSED_DATA preparsedData;
			if (!HidD_GetPreparsedData(m_read, &preparsedData)) throw HIDP_STATUS_INVALID_PREPARSED_DATA;
			if (HidP_GetCaps(preparsedData, &m_capabilities) == HIDP_STATUS_INVALID_PREPARSED_DATA) throw HIDP_STATUS_INVALID_PREPARSED_DATA;;
			if (!HidD_FreePreparsedData(preparsedData)) throw HIDP_STATUS_INVALID_PREPARSED_DATA;
			// Create the read thread
			if ((m_thread = (HANDLE)_beginthread(&WiimoteHidWin32::Run, 0, (void*)this)) == INVALID_HANDLE_VALUE) throw GetLastError();;
			
			// TODO: Make sure we have a live Wiimote, not a zombie (disconnected or powered down)
		}
		catch (DWORD)
		{
			CloseHandle(m_read);
			CloseHandle(m_write);
			CloseHandle(m_thread);
			throw;
		}
	}

	WiimoteHidWin32::~WiimoteHidWin32()
	{
		// Kill the read thread (as soon as it's done with the critical section)
		EnterCriticalSection(&m_criticalSection);
		TerminateThread(m_thread, NULL);
		LeaveCriticalSection(&m_criticalSection);
		// Close the thread handle
		CloseHandle(m_thread);
		// Delete the CriticalSection
		DeleteCriticalSection(&m_criticalSection);
		// Close the file handles
		CloseHandle(m_read);
		CloseHandle(m_write);
	}

	void WiimoteHidWin32::Run(void* wiimoteHid_)
	{
		// CONSIDER: What do we do with reports from a Wiimote that the application doesn't care about?
		WiimoteHidWin32* WiimoteHid = (WiimoteHidWin32*)wiimoteHid_;
		DWORD bytesRead = 0;
		size_t bufferSize = WIIMOTE_MAX_PAYLOAD + 1;
		u8 buffer[WIIMOTE_MAX_PAYLOAD + 1];
		
		for(;;)
		{
			// Read one report from the file
			if (!ReadFile(WiimoteHid->m_read, buffer, bufferSize, &bytesRead, NULL)) throw GetLastError();
			// Push the report onto some kind of report queue
			EnterCriticalSection(&WiimoteHid->m_criticalSection);
			WiimoteHid->m_reportQueue.push(*((Report*)buffer));
			LeaveCriticalSection(&WiimoteHid->m_criticalSection);
		}
	}

	void WiimoteHidWin32::WriteReport(u8* report_, size_t length)
	{
		DWORD bytesWritten;
		u8 report[WIIMOTE_MAX_PAYLOAD];
		memset(report, 0, WIIMOTE_MAX_PAYLOAD);
		memcpy(report, report_, length);
		if (!WriteFile(m_write, report, WIIMOTE_MAX_PAYLOAD, &bytesWritten, NULL)) throw GetLastError();
	}

	bool WiimoteHidWin32::GetNextReport(Report& report)
	{
		// If the report queue is empty return false
		if (m_reportQueue.empty()) return false;
		
		// Otherwise fill the given report and return true
		report = m_reportQueue.front();
		m_reportQueue.pop();
		return true;
	}
}
