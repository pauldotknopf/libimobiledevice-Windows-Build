// libimobiledevicetest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <libimobiledevice/libimobiledevice.h>
#include <libimobiledevice/lockdown.h>
#include <libimobiledevice/afc.h>
#include <libimobiledevice/house_arrest.h>

idevice_t phone = NULL;

int _tmain(int argc, _TCHAR* argv[])
{
	idevice_new(&phone, NULL);

	return 0;
}

