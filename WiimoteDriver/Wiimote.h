#pragma once
#include "WiimoteDefines.h"
#include "WiimoteCallback.h"

namespace WiimoteDriver
{
	class Wiimote
	{
	public:
		Wiimote(WiimoteCallback* callback) : m_callback(callback), m_uid(++s_uidCounter) {}
		virtual ~Wiimote() {}
		
		// Process any incoming reports for this Wiimote
		virtual void Process() = 0;
		// Get the UID for this Wiimote
		size_t GetUid() { return m_uid; }
		// Get an array of IrDots
		IrDot* GetDots() { return m_dots; }
		
		// Adjust the LEDs on the Wiimote (bitmask of WIIMOTE_LED_)
		virtual void SetLeds(u8 leds) = 0;
		// Request the battery level
		virtual void RequestBattery() = 0;
		// Set mode to allow for IR and accelorometer data
		virtual void IrMode() = 0;
		// Set mode to low-power, no IR or accelorometer
		virtual void LowPowerMode() = 0;
		
	protected:
		// When events come in from the wiimote callback to this object
		WiimoteCallback* m_callback;
		// Keep this populated with the latest IR information
		IrDot m_dots[4];
		
	private:
		// Static counter for Wiimote UIDs (starts assigning at 1)
		static size_t s_uidCounter;
		
		// Every Wiimote has a UID assocaited with it
		size_t m_uid;
	};
}
