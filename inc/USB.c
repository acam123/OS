#include "USB.h" 

usb_language_id_defs usb_language_id = {
	.ENGLISH_US = 0x0409
	//...
};

usb_standard_requests_defs usb_standard_requests = {
	.GET_STATUS = 0,
	.CLEAR_FEATURE = 1,
	.SET_FEATURE = 3,
	.SET_ADDRESS = 5,
	.GET_DESCRIPTOR = 6,
	.SET_DESCRIPTOR = 7,
	.GET_CONFIGURATION = 8,
	.SET_CONFIGURATION = 9,
	.GET_INTERFACE = 10,
	.SET_INTERFACE = 11,
	.SYNC_FRAME = 12
};

b_descriptor_types_defs b_descriptor_types = {
 	.DEVICE = 1,
	.CONFIGURATION = 2,
	.STRING = 3,
	.INTERFACE = 4,
	.ENDPOINT = 5,
	.DEVICE_QUALIFIER = 6,
	.OTHER_SPEED_CONFIG = 7,
	.INTERFACE_POWER = 8
};

b_descriptor_sizes_defs  b_descriptor_sizes = {
	.DEVICE = 0x12, //18
	.CONFIGURATION = 0x9, //9
	//.STRING = ,
	//.INTERFACE = ,
	.ENDPOINT = 0x7, //7
	.DEVICE_QUALIFIER = 0xa, //10
	.OTHER_SPEED_CONFIG = 0x9 //9
	//.INTERFACE_POWER = 
};

usb_transfer_directions_defs usb_transfer_directions = {
	.HOST_TO_DEVICE = 0, 
	.DEVICE_TO_HOST = 1
};

usb_request_types_defs usb_request_types = {
	.STANDARD = 0, 
	.CLASS = 1, 
	.VENDOR = 2, 
	.RESERVED = 3
};

usb_recipients_defs usb_recipients = { 
	.DEVICE = 0, 
	.INTERFACE = 1, 
	.ENDPOINT = 2, 
	.OTHER = 3
};


void init_usb() { 


}