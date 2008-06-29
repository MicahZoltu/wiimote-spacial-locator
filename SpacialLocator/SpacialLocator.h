#pragma once

// In theory, the coodinate 0,0,0 should be the center of the screen.
// Positive Z is away from the screen.
// Positive X is to the right.
// Positive Y is up.

namespace SpacialLocation
{
	class SpacialLocator
	{
	public:
		virtual ~SpacialLocator() {}
		
		// Let the library process data (call once per frame)
		virtual void Process() = 0;
		
		// Request the current coordinates from the library
		virtual float GetPitch() = 0;
		virtual float GetYaw() = 0;
		virtual float GetRoll() = 0;
	};
}
