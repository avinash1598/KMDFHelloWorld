/*
	This is WDF(Windows driver framework) based model

	For more information on WDF and WDM based callout models, visit:
	https://docs.microsoft.com/en-us/windows-hardware/drivers/network/creating-a-device-object
*/

#include <ntddk.h>
#include <wdf.h>

//#define TAG "KdfHelloWorld";

// declarations for Driver entry and Event device add
DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD KmdfHelloWorldEvtDeviceAdd;

//Define main or DriverEntry function
NTSTATUS DriverEntry(
	_In_ PDRIVER_OBJECT DriverObject,
	_In_ PUNICODE_STRING RegistryPath
)
{
	//possible values: success, info, warning, error
	NTSTATUS status = STATUS_SUCCESS;

	// Allocate the driver configuration object
	WDF_DRIVER_CONFIG config;

	// Print "Hello World" for DriverEntry
	KdPrintEx(( DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "KmdfHelloWorld: DriverEntry\n" ));
	KdPrint(("KmdfHelloWorld: KdPrint -> DriverEntry\n"));

	// Initialize the driver configuration object to register the
	// entry point for the EvtDeviceAdd callback, KmdfHelloWorldEvtDeviceAdd
	WDF_DRIVER_CONFIG_INIT(&config,
						   KmdfHelloWorldEvtDeviceAdd
						   );

	// Finally, create the driver object
	status = WdfDriverCreate(DriverObject,
							 RegistryPath,
							 WDF_NO_OBJECT_ATTRIBUTES,
						     &config,
							 WDF_NO_HANDLE
							 );

	return status;
}

NTSTATUS KmdfHelloWorldEvtDeviceAdd(
	_In_ WDFDRIVER Driver,
	_In_ PWDFDEVICE_INIT DeviceInit
) 
{
	// We're not using the driver object,
	// so we need to mark it as unreferenced
	UNREFERENCED_PARAMETER(Driver);

	NTSTATUS status;

	// Allocate the device object
	WDFDEVICE hDevice;

	//Print hello message from device add callback
	KdPrintEx(( DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "KmdfHelloWorld: KmdfHellowWorldEvtDeviceAdd\n" ));
	KdPrint(("KmdfHelloWorld: KdPrint -> KmdfHellowWorldEvtDeviceAdd\n"));

	//crreate the device object
	status = WdfDeviceCreate(&DeviceInit,
							 WDF_NO_OBJECT_ATTRIBUTES,
							 &hDevice);

	return status; 
}