#pragma once
#include "Wiimote.h"

#define WIIMOTE_MAX_PAYLOAD 0x16

namespace SpacialLocation
{
	// Report is packed since we will be typecasting a byte array to a report
	#pragma pack(push, 1)
	class Report
	{
	public:
		unsigned char m_id;
		unsigned char m_payload[WIIMOTE_MAX_PAYLOAD];
	};
	#pragma pack(pop)
	
	class WiimoteDevice : public Wiimote
	{
	public:
		WiimoteDevice(WiimoteCallback* callback);
		
		// Process any incoming reports for this Wiimote
		virtual void Process();
		// Adjust the LEDs on the Wiimote (bitmask of WIIMOTE_LED_)
		virtual void SetLeds(u8 leds);
		// Request the battery level
		virtual void RequestBattery();
		// Set mode to allow for IR and accelorometer data
		virtual void IrMode();
		// Set mode to low-power, no IR or accelorometer
		virtual void LowPowerMode();
		
	private:
		// Get the next report off the report queue; returns false if there are no more reports
		virtual bool GetNextReport(Report& report) = 0;
		// Helper function for report writing
		void WriteReport(u8 reportId, u8* reportData, size_t reportLength);
		// Write a report to the Wiimote
		virtual void WriteReport(u8* report, size_t length) = 0;
		
	private:
		// We keep track of button state inside the WiimoteDevice so we can make the appropriate callbacks
		u16 m_buttonState;
	};
}
