#include "WiimoteHidWin32Collection.h"
#include "WiimoteHidWin32.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <setupapi.h>
#include <process.h>
#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "hid.lib")
extern "C"
{
	#pragma warning(push)
	#pragma warning(disable:4201) // nonstandard extension used : nameless struct/union
	#include <hidsdi.h>
	#pragma warning(pop)
}

namespace WiimoteDriver
{
	WiimoteHidWin32Collection::WiimoteHidWin32Collection(WiimoteCallback* callback) :
			WiimoteCollection(callback),
			m_nextWiimoteSearch(GetTickCount())
	{
		
	}

	WiimoteHidWin32Collection::~WiimoteHidWin32Collection()
	{
		// Delete all of the wiimote devices
		for (std::map<std::wstring, WiimoteHid*>::iterator i = m_wiimoteHids.begin(); i != m_wiimoteHids.end(); ++i)
		{
			if (i->second) delete i->second;
			i->second = 0;
		}
	}

	HANDLE WiimoteHidWin32Collection::ConnectToDevice(DWORD index, std::wstring& devicePath)
	{
		GUID hidGUID;
		HDEVINFO deviceInfoSet;
		SP_DEVICE_INTERFACE_DATA deviceInterfaceData;
		PSP_INTERFACE_DEVICE_DETAIL_DATA deviceDetail;
		ULONG requiredSize;
		HANDLE deviceHandle = INVALID_HANDLE_VALUE;
		DWORD result;
		
		//Get the HID GUID value - used as mask to get list of devices
		HidD_GetHidGuid(&hidGUID);
		
		//Get a list of devices matching the criteria (hid interface, present)
		deviceInfoSet = SetupDiGetClassDevs(&hidGUID, NULL, NULL, (DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));
		
		deviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
		
		//Go through the list and get the interface data
		result = SetupDiEnumDeviceInterfaces(deviceInfoSet, NULL, &hidGUID, index, &deviceInterfaceData);
		
		/* Failed to get a device - possibly the index is larger than the number of devices */
		if (result == FALSE)
		{
			SetupDiDestroyDeviceInfoList (deviceInfoSet);
			return INVALID_HANDLE_VALUE;
		}
		
		//Get the details with null values to get the required size of the buffer
		SetupDiGetDeviceInterfaceDetail(deviceInfoSet, &deviceInterfaceData, NULL, 0, &requiredSize, 0);
		
		//Allocate the buffer
		deviceDetail = (PSP_INTERFACE_DEVICE_DETAIL_DATA)malloc(requiredSize);
		deviceDetail->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);
		
		//Fill the buffer with the device details
		if (!SetupDiGetDeviceInterfaceDetail(deviceInfoSet, &deviceInterfaceData, deviceDetail, requiredSize, &requiredSize, NULL)) 
		{
			SetupDiDestroyDeviceInfoList (deviceInfoSet);
			free (deviceDetail);
			return INVALID_HANDLE_VALUE;
		}
		
		// Save the device path in one of the out parameters
		devicePath = deviceDetail->DevicePath;
		
		//Open file on the device
		DWORD shareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
		DWORD disposition = OPEN_EXISTING;
		DWORD flags = FILE_FLAG_OVERLAPPED;
		deviceHandle = CreateFile(deviceDetail->DevicePath, 0, shareMode, NULL, disposition, flags, NULL);
		
		SetupDiDestroyDeviceInfoList(deviceInfoSet);
		free(deviceDetail);
		return deviceHandle;
	}

	int WiimoteHidWin32Collection::FindAllWiimotes()
	{
		// Don't search more then once every 30 seconds
		if (GetTickCount() < m_nextWiimoteSearch) return 0;
		m_nextWiimoteSearch += 30000;
		
		int numberFound = 0;
		HANDLE handle = INVALID_HANDLE_VALUE;
		DWORD index = 0;
		HIDD_ATTRIBUTES deviceAttributes;
		std::wstring devicePath;
		while ((handle = ConnectToDevice(index, devicePath)) != INVALID_HANDLE_VALUE)
		{
			if (!HidD_GetAttributes(handle, &deviceAttributes)) continue;
			
			if ((deviceAttributes.VendorID == WIIMOTE_VENDOR_ID) &&
					(deviceAttributes.ProductID == WIIMOTE_PRODUCT_ID) &&
					(m_wiimoteHids.count(devicePath) == 0))
			{
				// Create a WiimoteDevice
				WiimoteHid* wiimote = new WiimoteHidWin32(m_callback, devicePath);
				m_wiimoteHids[devicePath] = wiimote;
				m_wiimotes[wiimote->GetUid()] = wiimote;
				++numberFound;
			}
			
			CloseHandle(handle);
			++index;
		}
		
		return numberFound;
	}

	void WiimoteHidWin32Collection::Process()
	{
		for (std::map<size_t, Wiimote*>::iterator i = m_wiimotes.begin(); i != m_wiimotes.end(); ++i)
		{
			i->second->Process();
		}
	}

	Wiimote* WiimoteHidWin32Collection::GetWiimote(size_t wiimoteUid)
	{
		if (m_wiimotes.count(wiimoteUid) > 0) return m_wiimotes[wiimoteUid];
		else return 0;
	}

	Wiimote* WiimoteHidWin32Collection::GetNextWiimote(size_t& uid)
	{
		// If the user passed in 0 then start over
		if (uid == 0) m_wiimoteIterator = m_wiimotes.begin();
		else ++m_wiimoteIterator;
		
		// If we are out of Wiimotes reset our iterator and return 0
		if (m_wiimoteIterator == m_wiimotes.end())
		{
			m_wiimoteIterator = m_wiimotes.begin();
			return 0;
		}
		
		// Set the UID to that of the Wiimote we are returning
		uid = m_wiimoteIterator->second->GetUid();
		
		// Return the next Wiimote
		return m_wiimoteIterator->second;
	}
}
