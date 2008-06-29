#pragma once

#include "WiimoteDevice.h"

#define WIIMOTE_VENDOR_ID 0x057e
#define WIIMOTE_PRODUCT_ID 0x0306

namespace SpacialLocation
{
	class WiimoteHid
	{
	public:
		WiimoteHid(WiimoteCallback* callback) : m_callback(callback) {}
		virtual ~WiimoteHid() {};
		
		// Find all attached Wiimotes; returns the number of new devices found
		virtual int FindAllWiimotes() = 0;
		
		// Process all Wiimotes for incoming reports
		virtual void Process() = 0;
		
		// Get a Wiimote given an ID
		virtual Wiimote* GetWiimote(size_t wiimoteUid) = 0;
		
		// Iterate through all Wiimotes; initialize uid to 0 to start at the begining
		virtual Wiimote* GetNextWiimote(size_t& uid) = 0;
		
	protected:
		WiimoteCallback* m_callback;
	};
}
