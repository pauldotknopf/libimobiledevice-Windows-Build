// libimobiledevicetest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <libimobiledevice/libimobiledevice.h>
#include <libimobiledevice/lockdown.h>
#include <libimobiledevice/afc.h>
#include <libimobiledevice/house_arrest.h>

#define AFC_SERVICE_NAME "com.apple.afc2"
#define HOUSE_ARREST_SERVICE_NAME "com.apple.mobile.house_arrest"

idevice_t phone = NULL;
lockdownd_client_t control = NULL;
lockdownd_service_descriptor_t service = NULL;
house_arrest_client_t house_arrest = NULL;

int _tmain(int argc, _TCHAR* argv[])
{
	int res = EXIT_FAILURE;
	lockdownd_error_t lockdownResult = LOCKDOWN_E_SUCCESS;
	house_arrest_error_t houseArrestResult = HOUSE_ARREST_E_SUCCESS;

	idevice_new(&phone, NULL);

	lockdownResult = lockdownd_client_new_with_handshake(phone, &control, "ifuse");

	lockdownResult = lockdownd_start_service(control, HOUSE_ARREST_SERVICE_NAME, &service);

	houseArrestResult = house_arrest_client_new(phone, service, &house_arrest);

	if (!house_arrest) {
		fprintf(stderr, "Could not start document sharing service!\n");
		return EXIT_FAILURE;
	}

	houseArrestResult = house_arrest_send_command(house_arrest, "VendDocuments", "com.adobe.Adobe-Reader");

	plist_t dict = NULL;
	houseArrestResult = house_arrest_get_result(house_arrest, &dict);

	plist_t node = plist_dict_get_item(dict, "Error");
	if (node) {
		char *str = NULL;
		plist_get_string_val(node, &str);
		fprintf(stderr, "ERROR: %s\n", str);
		if (str) free(str);
		return EXIT_FAILURE;
	}
	plist_free(dict);

	return 0;
}

