// libimobiledevicetest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <assert.h>

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

static void device_subscribed(const idevice_event_t *event, void *user_data)
{
	std::cout << "Subcribe!" << std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	//idevice_event_subscribe(&device_subscribed, NULL);

	int res = EXIT_FAILURE;
	lockdownd_error_t lockdownResult = LOCKDOWN_E_SUCCESS;
	house_arrest_error_t houseArrestResult = HOUSE_ARREST_E_SUCCESS;
	afc_error_t afcResult = AFC_E_SUCCESS;

	idevice_new(&phone, NULL);
	assert(phone != NULL);

	lockdownResult = lockdownd_client_new_with_handshake(phone, &control, "ifuse");
	assert(lockdownResult == LOCKDOWN_E_SUCCESS);

	lockdownResult = lockdownd_start_service(control, HOUSE_ARREST_SERVICE_NAME, &service);

	houseArrestResult = house_arrest_client_new(phone, service, &house_arrest);
	assert(houseArrestResult == HOUSE_ARREST_E_SUCCESS);
	assert(house_arrest != NULL);

	houseArrestResult = house_arrest_send_command(house_arrest, "VendDocuments", "com.adobe.Adobe-Reader");
	assert(houseArrestResult == HOUSE_ARREST_E_SUCCESS);

	plist_t dict = NULL;
	houseArrestResult = house_arrest_get_result(house_arrest, &dict);
	assert(houseArrestResult == HOUSE_ARREST_E_SUCCESS);

	plist_t node = plist_dict_get_item(dict, "Error");
	if (node) {
		char *str = NULL;
		plist_get_string_val(node, &str);
		fprintf(stderr, "ERROR: %s\n", str);
		if (str) free(str);
		return EXIT_FAILURE;
	}
	plist_free(dict);

	//lockdownResult = lockdownd_client_free(control);

	afc_client_t afc = NULL;
	afcResult = afc_client_new_from_house_arrest_client(house_arrest, &afc);
	assert(afcResult == AFC_E_SUCCESS);

	afcResult = afc_make_directory(afc, "/Documents/test");
	assert(afcResult == AFC_E_SUCCESS);

	char **dirs = NULL;
	afcResult = afc_read_directory(afc, "/Documents", &dirs);
	assert(afcResult == AFC_E_SUCCESS);
	
	return 0;
}

