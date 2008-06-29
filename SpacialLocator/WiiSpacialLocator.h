#pragma once

#include "SpacialLocator.h"
#include "WiimoteCallback.h"
#include "Wiimote.h"
#include "WiimoteCollection.h"
#include <map>

namespace SpacialLocation
{
	class WiiSpacialLocator : public SpacialLocator, public WiimoteDriver::WiimoteCallback
	{
	public:
		WiiSpacialLocator(WiimoteDriver::WiimoteCollection* wiimoteCollection) : m_wiimoteCollection(wiimoteCollection) {}
		
		// Let the library process data (call once per frame)
		virtual void Process();
		
		// Request the current coordinates from the library
		virtual float GetPitch();
		virtual float GetYaw();
		virtual float GetRoll();
		
		// Called when a button is pressed.
		virtual void UpPressed(size_t id);
		virtual void LeftPressed(size_t id);
		virtual void RightPressed(size_t id);
		virtual void DownPressed(size_t id);
		virtual void APressed(size_t id);
		virtual void BPressed(size_t id);
		virtual void PlusPressed(size_t id);
		virtual void MinusPressed(size_t id);
		virtual void HomePressed(size_t id);
		virtual void OnePressed(size_t id);
		virtual void TwoPressed(size_t id);
		// Called when a button is released.
		virtual void UpReleased(size_t id);
		virtual void LeftReleased(size_t id);
		virtual void RightReleased(size_t id);
		virtual void DownReleased(size_t id);
		virtual void AReleased(size_t id);
		virtual void BReleased(size_t id);
		virtual void PlusReleased(size_t id);
		virtual void MinusReleased(size_t id);
		virtual void HomeReleased(size_t id);
		virtual void OneReleased(size_t id);
		virtual void TwoReleased(size_t id);
		// Contains information about the four IR dots visible by the Wiimote
		virtual void IrMovement(size_t id, u8 dotId, const WiimoteDriver::IrDot& dot);
		// Contains the current batter charge as a number from 0-100
		virtual void Battery(size_t id, u8 charge);
		
	private:
		// Private so users don't call it on accident
		// Needed because we have a pointer to an abstract class (WiimoteHid)
		WiiSpacialLocator& operator=(const WiiSpacialLocator&);
		
		void FindWiimotes();
		
		// Map of Wiimotes
		std::map<size_t, WiimoteDriver::Wiimote*> m_wiimotes;
		// Wiimote HID object
		WiimoteDriver::WiimoteCollection* m_wiimoteCollection;
	};
}
