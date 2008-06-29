#pragma once

#include "WiiSpacialLocator.h"
#include "WiimoteHidWin32Collection.h"

//#pragma warning(disable:0000)
namespace SpacialLocation
{
	class WiiSpacialLocatorWin32 : public WiiSpacialLocator
	{
	public:
		WiiSpacialLocatorWin32() : WiiSpacialLocator(&m_wiimoteHidWin32Collection), m_wiimoteHidWin32Collection(this) {}
		
	private:
		WiimoteDriver::WiimoteHidWin32Collection m_wiimoteHidWin32Collection;
	};
}
