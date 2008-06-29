#pragma once
#include "WiimoteDevice.h"
#include <string>
#include <queue>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma comment(lib, "hid.lib")
extern "C"
{
	#pragma warning(push)
	#pragma warning(disable:4201) // nonstandard extension used : nameless struct/union
	#include <hidsdi.h>
	#pragma warning(pop)
}

namespace SpacialLocation
{
	class WiimoteDeviceWin32 : public WiimoteDevice
	{
	public:
		WiimoteDeviceWin32(WiimoteCallback* callback, std::wstring devicePath);
		virtual ~WiimoteDeviceWin32();
		
	private:
		// Get the next report off the report queue; returns false if there are no more reports
		virtual bool GetNextReport(Report& report);
		// Write a report to the Wiimote
		virtual void WriteReport(u8* report, size_t length);
		
		// Thread function to read reports from the device and push them onto the report queue
		static void Run(void* wiimoteDevice_);
		
		std::wstring m_devicePath;
		HANDLE m_read;
		HANDLE m_write;
		HANDLE m_thread;
		HIDP_CAPS m_capabilities;
		CRITICAL_SECTION m_criticalSection;
		std::queue<Report> m_reportQueue;
	};
}
