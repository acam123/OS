#ifndef _USB_COMMON_
#define _USB_COMMON_


#include "Typedefs.h"

/*
 * PSEUDO NAMSPACE DEFINITION STRUCTS (DEFINED IN USB.C)
 */
typedef struct _desc_endpnt_b_addr_t_fields_directions_defs {
	uint8_t OUT;
	uint8_t IN;
} desc_endpnt_b_addr_t_fields_directions_defs;
extern desc_endpnt_b_addr_t_fields_directions_defs desc_endpnt_b_addr_t_fields_directions;

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
extern uint8_t b_descriptor_sizes[];

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

typedef struct _desc_endpnt_bm_attr_trasnfer_t_defs {
	uint8_t CONTROL;
	uint8_t ISOCHRONOUS;
	uint8_t BULK;
	uint8_t INTERRUPT;
} desc_endpnt_bm_attr_trasnfer_t_defs;
extern desc_endpnt_bm_attr_trasnfer_t_defs desc_endpnt_bm_attr_trasnfer_t;

typedef struct _desc_endpnt_bm_attr_synch_t_defs {
	uint8_t NO_SYNCH;
	uint8_t ASYNCH;
	uint8_t ADAPTIVE;
	uint8_t SYNCH;
} desc_endpnt_bm_attr_synch_t_defs;
extern desc_endpnt_bm_attr_synch_t_defs desc_endpnt_bm_attr_synch_t;

typedef struct _desc_endpnt_bm_attr_usage_t_defs {
	uint8_t DATA;
	uint8_t FEEDBACK;
	uint8_t IMPLICIT;
	uint8_t RESERVED;
} desc_endpnt_bm_attr_usage_t_defs;
extern desc_endpnt_bm_attr_usage_t_defs desc_endpnt_bm_attr_usage_t;
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

typedef struct _bm_desc_config_attr_t {
	uint8_t reserved_0 		:5; // 0b00000
	uint8_t remote_wakeup 	:1; // 0 = not supported, 1 = supported
	uint8_t self_powered	:1; // 0 = bus powered, 1 = has local power source (if b_max_power != 0, may use bus power)
	uint8_t reserved_1		:1; // 0b1
} bm_desc_config_attr_t;

typedef struct _usb_descriptor_configuration {
	uint8_t b_length;
	uint8_t b_descriptor_type;
	uint16_t w_total_length;
	uint8_t b_num_interfaces;
	uint8_t b_configuration_value;
	uint8_t i_configuration;
	bm_desc_config_attr_t bm_attributes;
	uint8_t b_max_power;
} __attribute__((__packed__)) usb_descriptor_configuration;

typedef struct _usb_descriptor_interface {
	uint8_t b_length;
	uint8_t b_descriptor_type;
	uint8_t b_interface_number;
	uint8_t b_alternate_setting;
	uint8_t b_num_endpoints;
	uint8_t b_interface_class;
	uint8_t b_interface_subclass;
	uint8_t b_interface_protocol;
	uint8_t i_interface;
} __attribute__((__packed__)) usb_descriptor_interface;

typedef struct _desc_endpnt_bm_attr_t_fields {
	uint8_t transfer_type 	:2;
	uint8_t synch_type 		:2;
	uint8_t usage_type		:2;
	uint8_t reserved_0		:2; //0's
} desc_endpnt_bm_attr_t_fields;

typedef union _desc_endpnt_bm_attr_t {
	uint8_t bits;
	desc_endpnt_bm_attr_t_fields fields;
} desc_endpnt_bm_attr_t;

typedef struct _desc_endpnt_b_addr_t_fields {
	uint8_t endpoint_number :4;
	uint8_t reserved_0 		:3;
	uint8_t direction 		:1; // 0 = Out, 1 = In
} desc_endpnt_b_addr_t_fields;

typedef union _desc_endpnt_b_addr_t {
	uint8_t bits;
	desc_endpnt_b_addr_t_fields fields; 
} desc_endpnt_b_addr_t;

typedef struct _usb_descriptor_endpoint {
	uint8_t b_length;
	uint8_t b_descriptor_type;
	desc_endpnt_b_addr_t b_endpoint_address; //bitmap
	desc_endpnt_bm_attr_t bm_attributes; //bitmap
	uint16_t w_max_packet_size;
	uint8_t b_interval;
} __attribute__((__packed__)) usb_descriptor_endpoint;

typedef struct _usb_descriptor_device_qualifier {
	uint8_t b_length;
	uint8_t b_descriptor_type;
	uint16_t bcd_usb;
	uint8_t b_device_class;
	uint8_t b_device_subclass;
	uint8_t b_device_protocol;
	uint8_t b_max_packet_size;
	uint8_t b_num_configurations;
	uint8_t b_reserved; // 0's
} __attribute__((__packed__)) usb_descriptor_device_qualifier;


void init_usb();

#endif