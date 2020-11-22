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

/*
VOID EvtIoDeviceControl(
	_In_ WDFQUEUE     Queue,
	_In_ WDFREQUEST   Request,
	_In_ size_t       OutputBufferLength,
	_In_ size_t       InputBufferLength,
	_In_ ULONG        IoControlCode
)
{
	NTSTATUS    status;
	PMDL        pMdl;

	UNREFERENCED_PARAMETER(Queue);
	UNREFERENCED_PARAMETER(OutputBufferLength);
	UNREFERENCED_PARAMETER(InputBufferLength);
	UNREFERENCED_PARAMETER(IoControlCode);


	status = WdfRequestRetrieveInputWdmMdl(Request, &pMdl);

	WdfRequestComplete(Request, status);
}

VOID
EvtIoRead(
	_In_ WDFQUEUE     Queue,
	_In_ WDFREQUEST   Request,
	_In_ size_t       Length
)
{

	UNREFERENCED_PARAMETER(Length);
	UNREFERENCED_PARAMETER(Queue);

	WdfRequestComplete(Request, STATUS_SUCCESS);
}
*/

VOID 
EvtDeviceFileCreate(
	_In_ WDFDEVICE Device, 
	_In_ WDFREQUEST Request, 
	_In_ WDFFILEOBJECT FileObject
)
{
	UNREFERENCED_PARAMETER(FileObject);
	UNREFERENCED_PARAMETER(Device);
	KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "KmdfHelloWorld: File created.\n"));
	WdfRequestComplete(Request, STATUS_SUCCESS);
}

VOID EvtFileClose(
	_In_ WDFFILEOBJECT FileObject
)
{
	UNREFERENCED_PARAMETER(FileObject);
	KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "KmdfHelloWorld: File closed.\n"));
	//WDFDEVICE device = WdfFileObjectGetDevice(FileObject);
}

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
	WDF_FILEOBJECT_CONFIG fileConfig;
	WDF_OBJECT_ATTRIBUTES fileAttributes;

	// Allocate the device object
	WDFDEVICE hDevice;

	//Print hello message from device add callback
	KdPrintEx(( DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "KmdfHelloWorld: KmdfHellowWorldEvtDeviceAdd\n" ));
	KdPrint(("KmdfHelloWorld: KdPrint -> KmdfHellowWorldEvtDeviceAdd\n"));

	//Handle file events
	WDF_FILEOBJECT_CONFIG_INIT(
		&fileConfig,
		EvtDeviceFileCreate,
		EvtFileClose,
		WDF_NO_EVENT_CALLBACK
	);

	WDF_OBJECT_ATTRIBUTES_INIT(&fileAttributes);
	WdfDeviceInitSetFileObjectConfig(
		DeviceInit,
		&fileConfig,
		&fileAttributes
	);

	//crreate the device object
	status = WdfDeviceCreate(&DeviceInit,
							 WDF_NO_OBJECT_ATTRIBUTES,
							 &hDevice);

	if (NT_SUCCESS(status)) {
		return status;
	}
	/*
	// Setup IO queue to handle DeviceIOControl
	WDF_IO_QUEUE_CONFIG queueConfig;
	WDF_OBJECT_ATTRIBUTES attributes;
	WDFQUEUE queue;

	WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&queueConfig, WdfIoQueueDispatchManual);
	queueConfig.EvtIoDeviceControl = EvtIoDeviceControl;
	queueConfig.EvtIoRead = EvtIoRead;

	// Call in PASSIVE_LEVEL
	WDF_OBJECT_ATTRIBUTES_INIT(&attributes);
	attributes.ExecutionLevel = WdfExecutionLevelPassive;

	status = WdfIoQueueCreate(
		hDevice,
		&queueConfig,
		&attributes,
		&queue
	);
	*/
	return status; 
}