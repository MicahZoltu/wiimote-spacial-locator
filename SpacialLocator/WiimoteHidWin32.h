#pragma once

#include "WiimoteHid.h"

#include <map>
#include <string>

// We are so close to not having to include windows.h...
typedef unsigned long DWORD;
typedef void* HANDLE;

namespace SpacialLocation
{
	class WiimoteHidWin32 : public WiimoteHid
	{
	public:
		WiimoteHidWin32(WiimoteCallback* callback);
		virtual ~WiimoteHidWin32();
		
		// Find all attached Wiimotes; returns true if any new wiimotes were found
		virtual int FindAllWiimotes();
		
		// Process all Wiimotes for incoming reports
		virtual void Process();
		
		// Get a Wiimote given an ID
		virtual Wiimote* GetWiimote(size_t wiimoteUid);
		
		// Iterate through all Wiimotes; initialize uid to 0 to start at the begining
		virtual Wiimote* GetNextWiimote(size_t& uid);
		
	private:
		// A map of Wiimote device paths to WiimoteDevice objects
		std::map<std::wstring, WiimoteDevice*> m_wiimoteDevices;
		// A map of Wiimote UIDs to Wiimotes
		std::map<size_t, Wiimote*> m_wiimotes;
		
		// Timer to keep track of when our next scheduled Wiimote search should run
		DWORD m_nextWiimoteSearch;
		
		// When iterating through the Wiimotes with GetNextWiimote track which we are on with this
		std::map<size_t, Wiimote*>::iterator m_wiimoteIterator;
		
		// Helpers
		HANDLE ConnectToDevice(DWORD index, std::wstring& devicePath);
	};
}
