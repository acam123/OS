#ifndef _USB_COMMON_
#define _USB_COMMON_


#include "Typedefs.h"

/*
 * PSEUDO NAMSPACE DEFINITION STRUCTS (DEFINED IN USB.C)
 */
// Language IDs
typedef struct _usb_language_id_defs {
	uint16_t ENGLISH_US;
	//...
} usb_language_id_defs;
extern usb_language_id_defs usb_language_id;

// USB STANDARD REQUESTS (bRequests)
typedef struct _usb_standard_requests_defs {
	uint8_t GET_STATUS;
	uint8_t CLEAR_FEATURE;
	uint8_t SET_FEATURE;
	uint8_t SET_ADDRESS;
	uint8_t GET_DESCRIPTOR;
	uint8_t SET_DESCRIPTOR;
	uint8_t GET_CONFIGURATION;
	uint8_t SET_CONFIGURATION;
	uint8_t GET_INTERFACE;
	uint8_t SET_INTERFACE;
	uint8_t SYNC_FRAME;
} usb_standard_requests_defs;
extern usb_standard_requests_defs usb_standard_requests;

// DESCRIPTOR TYPES
typedef struct _b_descriptor_types_defs {
	uint8_t DEVICE;
	uint8_t CONFIGURATION;
	uint8_t STRING;
	uint8_t INTERFACE;
	uint8_t ENDPOINT;
	uint8_t DEVICE_QUALIFIER;
	uint8_t OTHER_SPEED_CONFIG;
	uint8_t INTERFACE_POWER;
} b_descriptor_types_defs;
extern b_descriptor_types_defs b_descriptor_types;

// RETURN SIZE OF DESCRIPTOR TYPE
typedef struct _b_descriptor_sizes_defs {
	uint8_t DEVICE;
	uint8_t CONFIGURATION;
	uint8_t STRING;
	uint8_t INTERFACE;
	uint8_t ENDPOINT;
	uint8_t DEVICE_QUALIFIER;
	uint8_t OTHER_SPEED_CONFIG;
	uint8_t INTERFACE_POWER;
} b_descriptor_sizes_defs;
extern b_descriptor_sizes_defs  b_descriptor_sizes;

typedef struct _usb_transfer_directions_defs { 
	uint8_t HOST_TO_DEVICE;
	uint8_t DEVICE_TO_HOST;
} usb_transfer_directions_defs; 
extern usb_transfer_directions_defs usb_transfer_directions;

typedef struct _usb_request_types_defs {
	uint8_t STANDARD;
	uint8_t CLASS;
	uint8_t VENDOR;
	uint8_t RESERVED;
} usb_request_types_defs;
extern usb_request_types_defs usb_request_types;

typedef struct _usb_recipients_defs {
	uint8_t DEVICE;
	uint8_t INTERFACE;
	uint8_t ENDPOINT;
	uint8_t OTHER;
} usb_recipients_defs;
extern usb_recipients_defs usb_recipients;
// END OF DEFINITION STRUCTS

/*
 * USB INTERFACE STRUCTS
 */
typedef struct _bm_request_type {
	uint8_t recipient 			:5; // 00000b = Device, 00001b = Interface, 00010b = Endpoint, 00011b = Other
	uint8_t request_type 		:2; // 00b = Standard, 01b = Class, 10b = Vendor, 11b = Reserved
	uint8_t transfer_direction 	:1; // 0 = host to device, 1 = device to host
} bm_request_type;

typedef struct _usb_device_request {
	bm_request_type  request_type;
	uint8_t  b_request;
	uint16_t w_value;
	uint16_t w_index;
	uint16_t w_length; 
} __attribute__((__packed__)) usb_device_request;

typedef struct _usb_descriptor_device {
	uint8_t  b_length;
	uint8_t  b_descriptor_type;
	uint16_t bcd_usb;
	uint8_t  b_device_class;
	uint8_t  b_device_subclass;
	uint8_t  b_device_protocol;
	uint8_t  b_max_packet_size;
	uint16_t id_vendor;
	uint16_t id_product;
	uint16_t bcd_device;
	uint8_t  i_manufacturer;
	uint8_t  i_product;
	uint8_t  i_serial_number;
	uint8_t  b_num_configurations;
} __attribute__((__packed__)) usb_descriptor_device;

void init_usb();

#endif