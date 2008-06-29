#include "WiimoteDevice.h"
#include <memory.h>

namespace SpacialLocation
{
	WiimoteDevice::WiimoteDevice(WiimoteCallback* callback) :
			Wiimote(callback),
			m_buttonState(0)
	{
		 
	}

	void WiimoteDevice::SetLeds(u8 leds)
	{
		WriteReport(WIIMOTE_REPORT_OUT_LED, &leds, 1);
	}

	void WiimoteDevice::RequestBattery()
	{
		u8 batteryRequest = 0;
		WriteReport(WIIMOTE_REPORT_OUT_BATTERY, &batteryRequest, 1);
	}

	void WiimoteDevice::IrMode()
	{
		// Switch the mode to full reporting
		u8 mode[2] = {0x00, 0x33};
		WriteReport(WIIMOTE_REPORT_OUT_MODE, mode, 2);
		// Enable the cameras
		u8 camera = 0x04;
		WriteReport(WIIMOTE_REPORT_OUT_IR1, &camera, 1);
		WriteReport(WIIMOTE_REPORT_OUT_IR2, &camera, 1);
		// Write 0x08 to register 0xb00030 (?why?)
		u8 controlRegister[6] = {0x04, 0xb0, 0x00, 0x30, 0x01, 0x08};
		WriteReport(WIIMOTE_REPORT_OUT_WRITE, controlRegister, 6);
		// Set the IR mode to 3
		controlRegister[3] = 0x33;
		controlRegister[5] = 0x03;
		WriteReport(WIIMOTE_REPORT_OUT_WRITE, controlRegister, 6);
		// Set the lights to "high"
		SetLeds(WIIMOTE_LED_1 | WIIMOTE_LED_2 | WIIMOTE_LED_3 | WIIMOTE_LED_4);
	}

	void WiimoteDevice::LowPowerMode()
	{
		// Switch to low reporting mode (just buttons)
		u8 mode[2] = {0x00, 0x30};
		WriteReport(WIIMOTE_REPORT_OUT_MODE, mode, 2);
		// Disable the cameras
		u8 camera = 0x00;
		WriteReport(WIIMOTE_REPORT_OUT_IR1, &camera, 1);
		WriteReport(WIIMOTE_REPORT_OUT_IR2, &camera, 1);
		// Set the lights to "low"
		SetLeds(0);
	}

	void WiimoteDevice::Process()
	{
		Report report;
		while (GetNextReport(report))
		{
			switch (report.m_id)
			{
				case WIIMOTE_REPORT_IN_DATA_BUTTONS_ACCEL_IR:
				{
					for (u8 i = 0; i < 4; ++i)
					{
						IrDot& dot = m_dots[i];
						u8* payload = &report.m_payload[5 + (i * 3)];
						
						if (payload[0] != 0xFF || payload[1] != 0xFF || payload[2] != 0xFF)
						{
							dot.x = payload[0];
							dot.x |= (((payload[2] >> 4) & 0x03) << 8);
							dot.y = payload[1];
							dot.y |= (((payload[2] >> 6) & 0x03) << 8);
							dot.size = payload[2] & 0x0F;
							
							m_callback->IrMovement(GetUid(), i, dot);
						}
					}
					// Fall through to button handling
				}
				case WIIMOTE_REPORT_IN_DATA_BUTTONS:
				{
					// Get the change in buttons from the last state
					u16 newState = report.m_payload[0] << 8 | report.m_payload[1];
					u16 stateDelta = m_buttonState ^ newState;
					u16 pressed = newState & stateDelta;
					u16 released = ~newState & stateDelta;
					size_t uid = GetUid();
					
					// Call the appropriate functions based on the change in buttons
					if (pressed & WIIMOTE_BUTTON_TWO) m_callback->TwoPressed(uid);
					if (pressed & WIIMOTE_BUTTON_ONE) m_callback->OnePressed(uid);
					if (pressed & WIIMOTE_BUTTON_B) m_callback->BPressed(uid);
					if (pressed & WIIMOTE_BUTTON_A) m_callback->APressed(uid);
					if (pressed & WIIMOTE_BUTTON_MINUS) m_callback->MinusPressed(uid);
					if (pressed & WIIMOTE_BUTTON_HOME) m_callback->HomePressed(uid);
					if (pressed & WIIMOTE_BUTTON_LEFT) m_callback->LeftPressed(uid);
					if (pressed & WIIMOTE_BUTTON_RIGHT) m_callback->RightPressed(uid);
					if (pressed & WIIMOTE_BUTTON_DOWN) m_callback->DownPressed(uid);
					if (pressed & WIIMOTE_BUTTON_UP) m_callback->UpPressed(uid);
					if (pressed & WIIMOTE_BUTTON_PLUS) m_callback->PlusPressed(uid);
					if (released & WIIMOTE_BUTTON_TWO) m_callback->TwoReleased(uid);
					if (released & WIIMOTE_BUTTON_ONE) m_callback->OneReleased(uid);
					if (released & WIIMOTE_BUTTON_B) m_callback->BReleased(uid);
					if (released & WIIMOTE_BUTTON_A) m_callback->AReleased(uid);
					if (released & WIIMOTE_BUTTON_MINUS) m_callback->MinusPressed(uid);
					if (released & WIIMOTE_BUTTON_HOME) m_callback->HomePressed(uid);
					if (released & WIIMOTE_BUTTON_LEFT) m_callback->LeftPressed(uid);
					if (released & WIIMOTE_BUTTON_RIGHT) m_callback->RightPressed(uid);
					if (released & WIIMOTE_BUTTON_DOWN) m_callback->DownPressed(uid);
					if (released & WIIMOTE_BUTTON_UP) m_callback->UpPressed(uid);
					if (released & WIIMOTE_BUTTON_PLUS) m_callback->PlusPressed(uid);
					
					// Save the new button state
					m_buttonState = newState;
					break;
				}
				case WIIMOTE_REPORT_IN_BATTERY:
				{
					// Charge is a value from 0 - 200, 200 being full
					m_callback->Battery(GetUid(), report.m_payload[5] / 2);
					break;
				}
			}
		}
	}

	void WiimoteDevice::WriteReport(u8 reportId, u8* reportData, size_t reportLength)
	{
		// Length of the report is reportId + data length
		size_t length = reportLength + 1;
		u8* report = new u8[length];
		
		// First byte in a report is the ID
		report[0] = reportId;
		// The ID is followed by the report data
		memcpy(&report[1], reportData, reportLength);
		// Call into the OS specific system to actually write the report
		WriteReport(report, length);
		// Cleanup
		delete report;
	}
}
