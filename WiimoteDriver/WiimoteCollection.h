#pragma once
#include "WiimoteCallback.h"
#include "Wiimote.h"

namespace WiimoteDriver
{
	class WiimoteCollection
	{
	public:
		WiimoteCollection(WiimoteCallback* callback) : m_callback(callback) {}
		virtual ~WiimoteCollection() {};
		
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
