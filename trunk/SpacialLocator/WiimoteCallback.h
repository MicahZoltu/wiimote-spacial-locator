#pragma once
#include "WiimoteDefines.h"

namespace SpacialLocation
{
	class IrDot
	{
	public:
		u16 x;
		u16 y;
		u8 size;
		
		IrDot() : x(0), y(0), size(0) {}
	};
	
	class WiimoteCallback
	{
	public:
		virtual ~WiimoteCallback() {}
		
		// Called when a button is pressed.
		virtual void UpPressed(size_t id) = 0;
		virtual void LeftPressed(size_t id) = 0;
		virtual void RightPressed(size_t id) = 0;
		virtual void DownPressed(size_t id) = 0;
		virtual void APressed(size_t id) = 0;
		virtual void BPressed(size_t id) = 0;
		virtual void PlusPressed(size_t id) = 0;
		virtual void MinusPressed(size_t id) = 0;
		virtual void HomePressed(size_t id) = 0;
		virtual void OnePressed(size_t id) = 0;
		virtual void TwoPressed(size_t id) = 0;
		// Called when a button is released.
		virtual void UpReleased(size_t id) = 0;
		virtual void LeftReleased(size_t id) = 0;
		virtual void RightReleased(size_t id) = 0;
		virtual void DownReleased(size_t id) = 0;
		virtual void AReleased(size_t id) = 0;
		virtual void BReleased(size_t id) = 0;
		virtual void PlusReleased(size_t id) = 0;
		virtual void MinusReleased(size_t id) = 0;
		virtual void HomeReleased(size_t id) = 0;
		virtual void OneReleased(size_t id) = 0;
		virtual void TwoReleased(size_t id) = 0;
		// Contains information about the four IR dots visible by the Wiimote
		virtual void IrMovement(size_t id, u8 dotId, const IrDot& dot) = 0;
		// Contains the current batter charge as a number from 0-100
		virtual void Battery(size_t id, u8 charge) = 0;
	};
}
