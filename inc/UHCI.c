#include "UHCI.h"

uhci_packet_t_defs uhci_packet_t = {
	.SETUP = 0x2d,
	.IN = 0x69,
	.OUT = 0xe1
};

uhci_port_tracker_t uhci_port_tracker[16];
uint8_t uhci_port_tracker_count; 

void init_uhci() {

	uint8_t dev_num = 0;
	pci_write(uhci_endpoints[dev_num].bus, uhci_endpoints[dev_num].dev, uhci_endpoints[dev_num].func, 0xc0, 0x8f00); // disable usb legacy bios support
	uint16_t bar_4 = (uint16_t) get_bar_4(uhci_endpoints[dev_num].bus, uhci_endpoints[dev_num].dev, uhci_endpoints[dev_num].func) & 0x0000fffc; // mask off last 2 bits

	uint8_t intr_line = get_interrupt_line(uhci_endpoints[dev_num].bus, uhci_endpoints[dev_num].dev, uhci_endpoints[dev_num].func);

	pci_write(uhci_endpoints[dev_num].bus, uhci_endpoints[dev_num].dev, uhci_endpoints[dev_num].func, 0x04, 0x05); // turn on pci i/o & bus mastering
	uint8_t pci_cmd = get_command(uhci_endpoints[dev_num].bus, uhci_endpoints[dev_num].dev, uhci_endpoints[dev_num].func);
	
	uint64_t uhci_frame_list = (uint64_t) request_page();
	uint32_t* uhci_frame_list_ptr = (uint32_t*) uhci_frame_list;
	//memset(0x00000001, (uint64_t*) uhci_frame_list, 1024); // fill frame list with invalid pointers 0x1
	uint32_t tmp = 0x00000001;
	for (int i=0; i<1024; i++) {
		memcpy((char*) &tmp, (char*) uhci_frame_list+i*4, 4);
	}

	uhci_controller_config(bar_4, (uint32_t) uhci_frame_list);

	uhci_port_tracker_count = 0;

	uhci_check_ports(bar_4);

	uhci_devices_config(bar_4);

/*
	- Get Device Descriptor (8_Byte_Request, 8_Byte_Packets, Device_0) -> Max_Packet_Size
	- Get Device Descriptor (Descriptor_Size_Request, Max_Packet_Size, Device_0) -> Full Descriptor Info (String_Indexes for iMan, iProd, Serial No; # of Configurations)
	- Set Address (0_Byte_Request, Max_Packet_Size, Device_0) 
	- Get String Descriptor (8_Byte_Request, Max_Packet_Size, Device_1, String_Index_0, Lang_0) -> Languages Supported
	- Get String Descriptor (8_Byte_Request, Max_Packet_Size, Device_1, String_Index_1, Lang_English) -> String Length
	- Get String Descriptor (String_Length, Max_Packet_Size, Device_1, String_Index_1, Lang_English) -> String

	- **Get Configuration (1_Byte_Request, Max_Packet_Size, Device_1 ) -> Default Config Number ???
	- Get Configuration Descriptor (9 bytes) -> Config_Total_Length (of config & it's interface(s), endpoint(s), other class & vendor specific descriptor types); Config_String_Index
	- Get Configuration Descriptor (Config_Total_Length & parse)
	- Get String Descriptor (Config_String_Index) -> String_Length
	- Get String Descriptor (Config_String_Index, String_Length) -> String
	- **Set Configuration 
*/	

	/*
	 * 8 bytes of Device Descriptor
	 */
	uint8_t max_packet_size = 0x08;

	uint8_t low_speed = 0;
	uint8_t device = 0;
	uint8_t endpoint = 0;
	uint8_t response_packet_sz = max_packet_size;
	uint8_t response_num_bytes = 0x08;
	uint8_t transfer_direction = usb_transfer_directions.DEVICE_TO_HOST;
	uint8_t request_type = usb_request_types.STANDARD;
	uint8_t recipient = usb_recipients.DEVICE;
	uint8_t b_request = usb_standard_requests.GET_DESCRIPTOR;
	uint16_t w_value = (b_descriptor_types.DEVICE << 8) | 0x00;
	uint16_t w_index = 0x0000;

	uint32_t response = uhci_create_request (low_speed, device, endpoint, response_packet_sz, response_num_bytes, transfer_direction, request_type, recipient, b_request, w_value, w_index, uhci_frame_list_ptr);
	
	//max_packet_size = (((usb_descriptor_device*)response)->b_max_packet_size);


	print_string("\n\r");
	print_string("\n\r");
	print_string("RESPONSE #1");
	print_string("\n\r");
	print_string(hex_to_str(*(uint32_t*)response));
	print_string("\n\r");
	print_string(hex_to_str(*(((uint32_t*)(response))+1)));
	
	/*
	 * SET ADDRESS
	 */
	
	uint8_t device_address = 0x01;

	low_speed = 0;
	device = 0;
	endpoint = 0;
	response_packet_sz = max_packet_size;
	response_num_bytes = 0;
	transfer_direction = usb_transfer_directions.HOST_TO_DEVICE;
	request_type = usb_request_types.STANDARD;
	recipient = usb_recipients.DEVICE;
	b_request = usb_standard_requests.SET_ADDRESS; 
	w_value = device_address; // set as device 1
	w_index = 0x0000;

	response = uhci_create_request (low_speed, device, endpoint, response_packet_sz, response_num_bytes, transfer_direction, request_type, recipient, b_request, w_value, w_index, uhci_frame_list_ptr);
	
	print_string("\n\r");
	print_string("RESPONSE #2");
	print_string("\n\r");
	print_string("ADDRESS SET!!!");

	/*
	 * Full Device Descriptor
	 */
	low_speed = 0;
	device = device_address;
	endpoint = 0;
	response_packet_sz = max_packet_size;
	response_num_bytes = b_descriptor_sizes[b_descriptor_types.DEVICE];
	transfer_direction = usb_transfer_directions.DEVICE_TO_HOST;
	request_type = usb_request_types.STANDARD;
	recipient = usb_recipients.DEVICE;
	b_request = usb_standard_requests.GET_DESCRIPTOR; 
	w_value = (b_descriptor_types.DEVICE << 8) | 0x00;
	w_index = 0x000;

	response = uhci_create_request (low_speed, device, endpoint, response_packet_sz, response_num_bytes, transfer_direction, request_type, recipient, b_request, w_value, w_index, uhci_frame_list_ptr);
	
	uint8_t i_product = (((usb_descriptor_device*)response)->i_product);

	print_string("\n\r");
	print_string("\n\r");
	print_string("RESPONSE #3");
	print_string("\n\r");
	//usb_descriptor_device* descriptor_device = (usb_descriptor_device*) uhci_response;
	print_string(hex_to_str(*(uint32_t*)response));
	print_string("\n\r");
	print_string(hex_to_str(*(((uint32_t*)(response))+1)));
	print_string("\n\r");
	print_string(hex_to_str(*(((uint32_t*)(response))+2)));
	print_string("\n\r");
	print_string(hex_to_str(*(((uint32_t*)(response))+3)));
	print_string("\n\r");
	print_string(hex_to_str(*(((uint32_t*)(response))+4)&0x0000ffff));

	//uint32_t num_configs = *(((uint32_t*)(uhci_response))+4);


	/*
	* Get Product String Length
	*/
	low_speed = 0;
	device = device_address;
	endpoint = 0;
	response_packet_sz = max_packet_size;
	response_num_bytes = 0x08;
	transfer_direction = usb_transfer_directions.DEVICE_TO_HOST;
	request_type = usb_request_types.STANDARD;
	recipient = usb_recipients.DEVICE;
	b_request = usb_standard_requests.GET_DESCRIPTOR; 
	w_value = (b_descriptor_types.STRING << 8) | i_product;
	w_index = usb_language_id.ENGLISH_US;

	response = uhci_create_request (low_speed, device, endpoint, response_packet_sz, response_num_bytes, transfer_direction, request_type, recipient, b_request, w_value, w_index, uhci_frame_list_ptr);
	
	uint8_t product_len = *(uint8_t*)response;

	print_string("\n\r");
	print_string("\n\r");
	print_string("RESPONSE #4");
	print_string("\n\r");
	print_string("Product Len: ");
	print_string("\n\r");
	print_string(hex_to_str(product_len));

	/*
	* Get Product String 
	*/
	low_speed = 0;
	device = device_address;
	endpoint = 0;
	response_packet_sz = max_packet_size;
	response_num_bytes = product_len;
	transfer_direction = usb_transfer_directions.DEVICE_TO_HOST;
	request_type = usb_request_types.STANDARD;
	recipient = usb_recipients.DEVICE;
	b_request = usb_standard_requests.GET_DESCRIPTOR; 
	w_value = (b_descriptor_types.STRING << 8) | i_product;
	w_index = usb_language_id.ENGLISH_US;

	response = uhci_create_request (low_speed, device, endpoint, response_packet_sz, response_num_bytes, transfer_direction, request_type, recipient, b_request, w_value, w_index, uhci_frame_list_ptr);
	
	print_string("\n\r");
	print_string("\n\r");
	print_string("RESPONSE #5");
	print_string("\n\r");
	for (int i=2; i<response_num_bytes; i++) {
		uint8_t* response_ptr = (uint8_t*) response;
		if (i%2==0) {
			uint8_t ch = hex_to_int((uint8_t) *(response_ptr+i));
			print_char(ch);
		}
	}	
	

	/*
	 * 9 Bytes of Config Descriptor
	 */

	low_speed = 0;
	device = device_address;
	endpoint = 0;
	response_packet_sz = max_packet_size;
	response_num_bytes = b_descriptor_sizes[b_descriptor_types.CONFIGURATION];
	transfer_direction = usb_transfer_directions.DEVICE_TO_HOST;
	request_type = usb_request_types.STANDARD;
	recipient = usb_recipients.DEVICE;
	b_request = usb_standard_requests.GET_DESCRIPTOR; 
	w_value = (b_descriptor_types.CONFIGURATION << 8) | 0x00; //0th config
	w_index = 0x0000;

	response = uhci_create_request (low_speed, device, endpoint, response_packet_sz, response_num_bytes, transfer_direction, request_type, recipient, b_request, w_value, w_index, uhci_frame_list_ptr);
	
	uint8_t config_len = (((usb_descriptor_configuration*)response)->b_length);

	print_string("\n\r");
	print_string("\n\r");
	print_string("RESPONSE #6");
	print_string("\n\r");
	usb_parse_config( (uint8_t*)response, (uint8_t)response_num_bytes );



	/*
	 * FULL CONFIG
	 */


	low_speed = 0;
	device = device_address;
	endpoint = 0;
	response_packet_sz = max_packet_size;
	response_num_bytes = config_len;
	transfer_direction = usb_transfer_directions.DEVICE_TO_HOST;
	request_type = usb_request_types.STANDARD;
	recipient = usb_recipients.DEVICE;
	b_request = usb_standard_requests.GET_DESCRIPTOR; 
	w_value = (b_descriptor_types.CONFIGURATION << 8) | 0x00; //0th config
	w_index = 0x0000;

	response = uhci_create_request (low_speed, device, endpoint, response_packet_sz, response_num_bytes, transfer_direction, request_type, recipient, b_request, w_value, w_index, uhci_frame_list_ptr);
	
	uint8_t i_configuration = (((usb_descriptor_configuration*)response)->i_configuration);

	print_string("\n\r");
	print_string("\n\r");
	print_string("RESPONSE #7");
	print_string("\n\r");
	usb_parse_config((uint8_t*)response, (uint8_t)response_num_bytes);


	
	/*
	 * Get Configuration String Length
	 */

	low_speed = 0;
	device = device_address;
	endpoint = 0;
	response_packet_sz = max_packet_size;
	response_num_bytes = 0x08;
	transfer_direction = usb_transfer_directions.DEVICE_TO_HOST;
	request_type = usb_request_types.STANDARD;
	recipient = usb_recipients.DEVICE;
	b_request = usb_standard_requests.GET_DESCRIPTOR;
	w_value = (b_descriptor_types.STRING << 8) | i_configuration; //
	w_index = usb_language_id.ENGLISH_US;

	malloc(16+15);

	response = uhci_create_request (low_speed, device, endpoint, response_packet_sz, response_num_bytes, transfer_direction, request_type, recipient, b_request, w_value, w_index, uhci_frame_list_ptr);

	uint8_t config_str_len = *(uint8_t*)response;

	print_string("\n\r");
	print_string("\n\r");
	print_string("RESPONSE #8");
	print_string("\n\r");
	print_string(hex_to_str(config_str_len));
	


	/*
	 * Get Configuration String
	 */

	low_speed = 0;
	device = device_address;
	endpoint = 0;
	response_packet_sz = max_packet_size;
	response_num_bytes = config_str_len;
	transfer_direction = usb_transfer_directions.DEVICE_TO_HOST;
	request_type = usb_request_types.STANDARD;
	recipient = usb_recipients.DEVICE;
	b_request = usb_standard_requests.GET_DESCRIPTOR;
	w_value = (b_descriptor_types.STRING << 8) | i_configuration; //
	w_index = usb_language_id.ENGLISH_US;

	malloc(16);

	response = uhci_create_request (low_speed, device, endpoint, response_packet_sz, response_num_bytes, transfer_direction, request_type, recipient, b_request, w_value, w_index, uhci_frame_list_ptr);

	print_string("\n\r");
	print_string("\n\r");
	print_string("RESPONSE #9");
	print_string("\n\r");

	for (int i=2; i<response_num_bytes; i++) {
		uint8_t* response_ptr = (uint8_t*) response;
		if (i%2==0) {
			uint8_t ch = hex_to_int((uint8_t) *(response_ptr+i));
			print_char(ch);
		}
	}	




}

uint8_t endian_flip (void* ptr_v, uint32_t sz) {
	uint8_t success = 1;
	if (sz == 0) { return success; }
	char* ptr = (char*) ptr_v;

	for (uint32_t i=0; i<(sz/2); i++) {
		uint8_t tmp = *(ptr+i);
		*(ptr+i) = *(ptr+sz-i-1);
		*(ptr+sz-i-1) = tmp;
	}

	success = 0;
	return success;
}

uint8_t uhci_controller_config (uint16_t base_addr, uint32_t frame_list_addr) {
	uint8_t success = 1;

	// reset 5 times
	for(int i=0; i<5; i++) {
		out_16(base_addr+USBCMD, 0x4); // uhci global reset 
		sleep(11.0);
		out_16(base_addr+USBCMD, 0x0); // uhci stop
	}

	out_16(base_addr+USBSTS, 0x3f); // clear uhci the status register (NOTE: software writing a 1 sets these bits to 0)

	out_16(base_addr+USBCMD, 0x02); // reset host controller
	sleep(42.0);

	out_32(base_addr+FRBASEADD, frame_list_addr); // set frame list base address 

	out_16(base_addr+USBINTR, 0x0); // disable interrupts

	out_8(base_addr+SOFMOD, 0x40); // reassert default for start of frame register upon uhci reset

	out_16(base_addr+FRNUM, 0x0); // frame number set to to default 0x0

	out_16(base_addr+USBCMD, 0x61); // uhci run, set configure semaphore, set software debug

	success = 0;
	return success;
}

uint8_t uhci_check_ports (uint16_t base_addr) {
	uint8_t success = 1;
	uint8_t ports[2] = {PORTSC1, PORTSC2};
	uint8_t sz = (sizeof(ports)/sizeof(ports[0]));
	for (uint8_t i=0; i<sz; i++) {
		if ( (in_16(base_addr+ports[i]) & 0x1) == 0x1 ) {
			uhci_port_tracker[uhci_port_tracker_count].port = ports[i];
			uhci_port_tracker_count++;
		}
	}
	success = 0;
	return success;
}

uint8_t uhci_devices_config (uint16_t base_addr) {
	uint8_t port;
	for (uint8_t i=0; i<uhci_port_tracker_count; i++ ) {
		port = uhci_port_tracker[i].port;

		// Reset Device
		uint8_t reset = uhci_device_reset(base_addr, port);

		// Get & store device speed
		uint8_t low_speed = uhci_device_low_speed(base_addr, port);
		if (low_speed == 1) {
			uhci_port_tracker[i].low_speed = 1;
		}

		// Enable Device
		uint8_t enabled = uhci_device_enable(base_addr, port);
	}
}

uint8_t uhci_device_reset (uint16_t base_addr, uint8_t port) {
	uint8_t success = 1;
	out_16(base_addr+port, 0x200); // reset device
	sleep(50.0);
	out_16(base_addr+port, in_16(base_addr+port) & ~(0x0200) ); // stop the reset
	sleep(10.0);
	success = 0;
	return success;
}

uint8_t uhci_device_low_speed (uint16_t base_addr, uint8_t port) {
	uint8_t low_speed = (in_16(base_addr+port) & 0x100) == 0x100 ? 1 : 0;
	return low_speed;
}

	
uint8_t uhci_device_enable (uint16_t base_addr, uint8_t port) {
	uint8_t success = 1;

	//enable device, clear status change, enable port
	out_16(base_addr+port, in_16(base_addr+port) | 0b1110);
	sleep(20.0);

	//verify enabled
	if ( (in_16(base_addr+port) & 0b0100) == 0b0100 ) {
		//enabled
		success = 0;
	}
	return success;
}


void print_td (uint32_t* td) {
	print_string("@");
	print_string(hex_to_str((uint32_t)td));
	print_string("\n\r");
	print_string(hex_to_str((uint32_t) *(td) ));
	print_string("\n\r");
	print_string(hex_to_str((uint32_t) *(td+1) ));
	print_string("\n\r");
	print_string(hex_to_str((uint32_t) *(td+2) ));
	print_string("\n\r");
	print_string(hex_to_str((uint32_t) *(td+3) ));
}

void print_qh (uint32_t* qh_ptr) {
	print_string("@");
	print_string(hex_to_str((uint32_t)qh_ptr));
	print_string("\n\r");
	print_string(hex_to_str(*(qh_ptr)));
	print_string("\n\r");
	print_string(hex_to_str(*(qh_ptr+1)));
	print_string("\n\r");
}

uint8_t hex_to_int (uint8_t hex) {
	uint8_t out = 0;
	uint8_t fst = (hex&0xf0)>>4;
	uint8_t lst = hex&0x0f;
	out = lst + fst*16;
	return out;
}

void usb_parse_config (uint8_t* buf, uint8_t sz) {
	uint8_t* orig = buf; 
	usb_descriptor_configuration* config;

	while( (buf-orig)<sz ) {
		uint8_t desc_len = *buf;
		uint8_t desc_type = *(buf+1);
		print_string("Type:");
		print_string(hex_to_str(desc_type));

		switch (desc_type) {
			case 2: //b_descriptor_types.CONFIGURATION:
				print_string("-config");
				break;
			case 4: //b_descriptor_types.INTERFACE:
				print_string("-interface");
				break;
			case 5: //b_descriptor_types.ENDPOINT:
				print_string("-endpoint");
				break;
			default:
				print_string("-other");
		}

		print_string(", Len:");
		print_string(hex_to_str(desc_len));
		print_string("\n\r");

		if ( desc_type == 2 ) {
			config = (usb_descriptor_configuration*) buf;
			print_string("  -");
			print_string("wTotalLength: ");
			print_string(hex_to_str(config->w_total_length));
			print_string("\n\r");
			print_string("  -");
			print_string("bNumInterfaces: ");
			print_string(hex_to_str(config->b_num_interfaces));
			print_string("\n\r");
			print_string("  -");
			print_string("bConfigurationValue: ");
			print_string(hex_to_str(config->b_configuration_value));
			print_string("\n\r");
			print_string("  -");
			print_string("iConfiguration: ");
			print_string(hex_to_str(config->i_configuration));
			print_string("\n\r");
		}

		buf += desc_len;
	}
}

uint32_t align16 (uint32_t sz) {
	return (sz % 16 == 0) ? sz : sz + 16 - (sz % 16); 
}

uint32_t uhci_create_request (uint8_t low_speed, uint8_t device, uint8_t endpoint, uint8_t response_packet_sz, uint8_t response_num_bytes, uint8_t transfer_direction, uint8_t request_type, uint8_t recipient, uint8_t b_request, uint16_t w_value, uint16_t w_index, uint32_t* uhci_frame_list_ptr) {
	uint8_t response_buf_packet = (response_num_bytes % response_packet_sz) == 0 ? 0 : 1; 
	uint8_t response_num_packets = response_num_bytes / response_packet_sz + response_buf_packet;

	uint32_t td_setup_orig = (uint32_t) malloc(sizeof(uhci_transfer_descriptor) + 16);
	uint32_t td_status_orig = (uint32_t) malloc(sizeof(uhci_transfer_descriptor) + 16);
	uhci_transfer_descriptor* td_setup = (uhci_transfer_descriptor*) align16(td_setup_orig);
	uhci_transfer_descriptor* td_status = (uhci_transfer_descriptor*) align16(td_status_orig);

	uint32_t tds_orig[response_num_packets];
	uhci_transfer_descriptor* tds[response_num_packets]; 
	//uhci_transfer_descriptor* tds[response_num_packets==0 ? 1 : response_num_packets]; 
	for (uint8_t i=0; i<response_num_packets; i++) { 
		 tds_orig[i] = (uint32_t) malloc(sizeof(uhci_transfer_descriptor) + 16);
		 tds[i] = (uhci_transfer_descriptor*) align16(tds_orig[i]);
		if ( tds[i] == 0 ) {
			return 0;
		}
	}

	uint32_t uhci_qh_orig = (uint32_t) malloc(16 + 16);
	uint32_t* uhci_qh = (uint32_t*) align16(uhci_qh_orig);

	uint32_t uhci_request = (uint32_t) malloc(8);

	if ( uhci_request == 0 | uhci_qh == 0 | td_setup == 0 | td_status == 0 ) {
		return 0;
	}

	uint32_t uhci_response = 0;
	if (response_num_bytes != 0) {
		uint32_t uhci_response = (uint32_t) malloc(response_num_bytes);
		if (uhci_response == 0) {
			return 0;
		}
	}

	// SETUP TD
	td_setup->next_descriptor = ( (uint32_t)(uint64_t)(response_num_bytes==0 ? td_status : tds[0]) )  | 0x4; // flip on depth bit
	td_setup->status.low_speed = low_speed;
	td_setup->status.active = 1;
	td_setup->status.error_counter = 3; 
	td_setup->packet_header.packet_type = uhci_packet_t.SETUP;
	td_setup->packet_header.device = device;
	td_setup->packet_header.endpoint = endpoint;
	td_setup->packet_header.data_toggle = 0b0;
	td_setup->packet_header.maximum_length = response_packet_sz - 1;
	td_setup->buffer_addr = uhci_request;

	// DATA TD's
	for (uint8_t i=0; i<response_num_packets; i++) {
		if ( i == (response_num_packets-1) ) {
			tds[i]->next_descriptor = ((uint32_t)(uint64_t)td_status) | 0x4;
		}
		else {
			tds[i]->next_descriptor = ((uint32_t)(uint64_t)tds[(i+1)]) | 0x4;
		}
		tds[i]->status.low_speed = low_speed;
		tds[i]->status.active = 1;
		tds[i]->status.error_counter = 3; 
		tds[i]->packet_header.packet_type = (transfer_direction==usb_transfer_directions.DEVICE_TO_HOST) ? uhci_packet_t.IN : uhci_packet_t.OUT;	
		tds[i]->packet_header.device = device; // Now we talk to the device we set up as device 1
		tds[i]->packet_header.endpoint = endpoint;
		tds[i]->packet_header.data_toggle = ((i%2==0) ? 0b1: 0b0);
		tds[i]->packet_header.maximum_length = response_packet_sz - 1;
		tds[i]->buffer_addr = (uhci_response + 8*(i));
	}

	//STATUS TD
	td_status->next_descriptor = 0x00000001;
	td_status->status.low_speed = low_speed;
	td_status->status.active = 1;
	td_status->status.error_counter = 3;
	td_status->packet_header.packet_type = (transfer_direction==usb_transfer_directions.DEVICE_TO_HOST) ? uhci_packet_t.OUT : uhci_packet_t.IN;		
	td_status->packet_header.device = device; // Now we talk to the device we set up as device 1
	td_status->packet_header.endpoint = endpoint;
	td_status->packet_header.data_toggle = 0b1;
	td_status->packet_header.maximum_length = 2047; // low_speed ? 63 : 2047; 
	td_status->buffer_addr = 0x0;

	// UHCI REQUEST
	usb_device_request* get_descriptor = (usb_device_request*)(uint64_t)(uhci_request);
	get_descriptor->request_type.transfer_direction = transfer_direction;
	get_descriptor->request_type.request_type = request_type;
	get_descriptor->request_type.recipient = recipient;
	get_descriptor->b_request = b_request;
	get_descriptor->w_value = w_value;
	get_descriptor->w_index = w_index;
	get_descriptor->w_length = (0x00 << 8) | response_num_bytes;

	// QH	
	*uhci_qh = (uint32_t)(uint64_t)td_status;// horizontal pointer to terminate // (uint32_t)(uint64_t)td_out; //horizontal pointer to td_out??? // 0x00000001
	*(uhci_qh+1) = (uint32_t)(uint64_t)td_setup; // vertical pointer to tds[0]
	*(uhci_qh+2) = 0x00000000;
	*(uhci_qh+3) = 0x00000000;
	
	// Add new QH to frame list
	*(uhci_frame_list_ptr) = ((uint32_t)(uint64_t)uhci_qh) | 0x2; // ptr to qh plus set as qh 

	print_string("\n\r");
	print_string("Transfer Setup Completed");

	uint8_t loops = 0;
	while (1) {
		if ( td_status->status.active == 0b0) {
			// On Success
			if ( (td_setup->status.actual_length == (response_packet_sz-1) )  ) {
				// Clear Queue Head from Frame List
				*uhci_frame_list_ptr = 0x1; 

				print_string("\n\r");
				print_string("Successful Transfer");
			}
			else {
				print_string("\n\r");
				print_string("Transfer Error");
			}
			break;
		}
		sleep(10);
		loops++;
		if (loops > 100) {
			print_string("\n\r");
			print_string("MAXED OUT!!!");
			break;
		}
	}

	/*
	if (response_packet_sz == 0x40) {
		print_string("\n\r");
		print_string("\n\r");
		print_string("!!! SETUP !!!");
		print_string("\n\r");
		print_td(td_setup);

		print_string("\n\r");
		print_string("\n\r");
		print_string("!!! IN !!!");
		print_string("\n\r");
		print_td(tds[0]);

		print_string("\n\r");
		print_string("\n\r");
		print_string("!!! STATUS !!!");
		print_string("\n\r");
		print_td(td_status);
		print_string("\n\r");

		print_string("\n\r");
		print_string("\n\r");
		print_string("!!! REQUEST !!!");
		print_string("\n\r");
		print_string(hex_to_str(*((uint32_t*)uhci_request)));
		print_string("\n\r");
		print_string(hex_to_str(*(((uint32_t*)uhci_request)+1)));
	}
	*/

	
	free((void*)td_setup_orig);
	free((void*)td_status_orig);
	for (uint8_t i=0; i<response_num_packets; i++) { 
		free((void*)tds_orig[i]);
	}
	free((void*)uhci_qh_orig);
	free((void*)uhci_request);
	
	return uhci_response;
}

// TO DO 
/*
 Larger Packet Sizes
 Set Configuration
 Pass Through USB
*/

