#ifndef _UHCI_
#define _UHCI_

#include "TextPrint.h"
#include "IO.h"
#include "Typedefs.h"
#include "PCI.h"
#include "PageTable.h"
#include "Heap.h"
#include "PIT.h"
#include "USB.h"

#define USBCMD 		0x0
#define USBSTS 		0x2
#define USBINTR 	0x4
#define FRNUM 		0x6
#define FRBASEADD 	0x8
#define SOFMOD 		0xc
#define PORTSC1 	0x10
#define PORTSC2 	0x12

extern pci_meta uhci_endpoints[10];

typedef struct _uhci_port_tracker_t {
	uint8_t port;
	uint8_t low_speed;
} uhci_port_tracker_t;

typedef struct _uhci_command_register {
	uint16_t run					:1;
	uint16_t host_controller_reset	:1;
	uint16_t global_reset			:1;
	uint16_t global_suspend			:1;
	uint16_t global_resume			:1;
	uint16_t software_debug			:1;
	uint16_t configure				:1;
	uint16_t max_packet				:1;
	uint16_t reserved				:8;
} uhci_command_register;

typedef struct _uhci_status_register {
	uint16_t interrupt 				:1;
	uint16_t error_interrupt		:1;
	uint16_t resume_detected		:1;
	uint16_t system_error			:1;
	uint16_t process_error 			:1;
	uint16_t halted 				:1;
	uint16_t reserved 				:10;
} uhci_status_register;

typedef struct _uhci_interrupt_enable_register {
	uint16_t timeout_crc 			:1;
	uint16_t resume 				:1;
	uint16_t complete 				:1;
	uint16_t short_packet 			:1;
	uint16_t reserved 				:12;
} uhci_interrupt_enable_register;

typedef struct _uhci_port_status_control_register {
	uint16_t connected 				:1;
	uint16_t connected_changed		:1;
	uint16_t port_enabled 			:1;
	uint16_t port_enable_changed  	:1;
	uint16_t line_status 			:2;
	uint16_t resume_detected 		:1;
	uint16_t reserved_0 			:1;
	uint16_t low_speed 				:1;
	uint16_t reset 					:1;
	uint16_t reserved_1 			:2;
	uint16_t suspend 				:1;
	uint16_t reserved_2 			:3;
} uhci_port_status_control_register;

typedef struct _uhci_io_registers {
	uhci_command_register usb_command; //uint16_t usb_command;
	uhci_status_register usb_status; //uint16_t usb_status;
	uhci_interrupt_enable_register usb_interrupt_enable; //uint16_t usb_interrupt_enable;
	uint16_t frame_number;
	uint32_t frame_list_base_addr;
	uint8_t start_of_frame_modify;
	uhci_port_status_control_register port_1; //uint16_t port_1_status_control;
	uhci_port_status_control_register port_2; //uint16_t port_2_status_control;
} __attribute__((__packed__)) uhci_io_registers;

typedef struct _uhci_frame_list_entry {
	uint32_t is_empty_frame 		:1;
	uint32_t td_or_qh 				:1;
	uint32_t reserved 				:2;
	uint32_t first_descriptor_addr 	:28;
} uhci_frame_list_entry;

typedef struct _uhci_descriptor_status {
	uint32_t actual_length 			:11;
	uint32_t reserved_0 			:6;
	uint32_t bit_stuff_error 		:1;
	uint32_t timeout_crc 			:1;
	uint32_t non_acknowledged 		:1;
	uint32_t babble_detected 		:1;
	uint32_t data_buffer_error 		:1;
	uint32_t stalled 				:1;
	uint32_t active 				:1;
	uint32_t interrupt_on_complete  :1;
	uint32_t is_isochronous 		:1;
	uint32_t low_speed 				:1;
	uint32_t error_counter 			:2;
	uint32_t short_packet_detected  :1;
	uint32_t reserved_1 			:2;
} uhci_descriptor_status;

typedef struct _uhci_descriptor_packet_header {
	uint32_t packet_type 			:8;
	uint32_t device 				:7;
	uint32_t endpoint 				:4;
	uint32_t data_toggle 			:1;
	uint32_t reserved 				:1;
	uint32_t maximum_length 		:11;
} uhci_descriptor_packet_header;

typedef struct _uhci_transfer_descriptor {
	uint32_t next_descriptor;
	uhci_descriptor_status status; //uint32_t status;
	uhci_descriptor_packet_header packet_header; //uint32_t packet_header;
	uint32_t buffer_addr;
	uint32_t reserved_0;
	uint32_t reserved_1;
	uint32_t reserved_2;
	uint32_t reserved_3;
} __attribute__((__packed__)) uhci_transfer_descriptor;

typedef struct _uhci_packet_defs {
	uint8_t SETUP;
	uint8_t IN;
	uint8_t OUT;
} uhci_packet_t_defs;
extern uhci_packet_t_defs uhci_packet_t; 


void init_uhci();

uint8_t endian_flip (void* ptr_v, uint32_t sz);

uint8_t uhci_controller_config (uint16_t base_addr, uint32_t frame_list_addr);

uint8_t uhci_check_ports (uint16_t base_addr);
uint8_t uhci_devices_config (uint16_t base_addr);
uint8_t uhci_device_reset (uint16_t base_addr, uint8_t port);
uint8_t uhci_device_low_speed (uint16_t base_addr, uint8_t port);
uint8_t uhci_device_enable (uint16_t base_addr, uint8_t port);
void print_td (uint32_t* td);
void print_qh (uint32_t* qh_ptr);
uint8_t hex_to_int (uint8_t hex);
void usb_parse_config(uint8_t* buf, uint8_t sz);
uint32_t align16 (uint32_t sz);
uint32_t uhci_create_request (uint8_t low_speed, uint8_t device, uint8_t endpoint, uint8_t response_packet_sz, uint8_t response_num_bytes, uint8_t transfer_direction, uint8_t request_type, uint8_t recipient, uint8_t b_request, uint16_t w_value, uint16_t w_index, uint32_t* uhci_frame_list_ptr);



#endif