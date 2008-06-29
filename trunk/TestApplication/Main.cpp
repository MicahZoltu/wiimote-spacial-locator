#include "WiiSpacialLocatorWin32.h"
#include <windows.h>

int main()
{
	SpacialLocation::SpacialLocator* spacialLocator = new SpacialLocation::WiiSpacialLocatorWin32();
	while (true)
	{
		spacialLocator->Process();
		wprintf(L"(%f,%f)\n", spacialLocator->GetYaw(), spacialLocator->GetPitch());
		// Don't eat CPU while idle
		Sleep(1);
	}
}
