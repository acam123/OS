#include "USB.h" 

desc_endpnt_b_addr_t_fields_directions_defs desc_endpnt_b_addr_t_fields_directions = {
	.OUT = 0,
	.IN = 1
};

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

uint8_t b_descriptor_sizes[] = {0,0x12,0x9,0,0x9,0x7,0xa,0x9,0};


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

desc_endpnt_bm_attr_trasnfer_t_defs desc_endpnt_bm_attr_trasnfer_t = {
	.CONTROL = 0b00,
	.ISOCHRONOUS = 0b01,
	.BULK = 0b10,
	.INTERRUPT = 0b11
};

desc_endpnt_bm_attr_synch_t_defs desc_endpnt_bm_attr_synch_t = {
	.NO_SYNCH = 0b00,
	.ASYNCH = 0b01,
	.ADAPTIVE = 0b10,
	.SYNCH = 0b11
};

desc_endpnt_bm_attr_usage_t_defs desc_endpnt_bm_attr_usage_t = {
	.DATA = 0b00,
	.FEEDBACK = 0b01,
	.IMPLICIT = 0b10,
	.RESERVED = 0b11
};


void init_usb() { 


}