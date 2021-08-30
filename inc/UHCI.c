#include "UHCI.h"

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

	// NEED 32 BIT ADDRESSES & Alignment for TD & QH
	
	
	uhci_transfer_descriptor* td_0 = (uhci_transfer_descriptor*) malloc(sizeof(uhci_transfer_descriptor));
	uhci_transfer_descriptor* td_1 = (uhci_transfer_descriptor*) malloc(sizeof(uhci_transfer_descriptor));
	uhci_transfer_descriptor* td_2 = (uhci_transfer_descriptor*) malloc(sizeof(uhci_transfer_descriptor));
	uhci_transfer_descriptor* td_3 = (uhci_transfer_descriptor*) malloc(sizeof(uhci_transfer_descriptor));
	uhci_transfer_descriptor* td_4 = (uhci_transfer_descriptor*) malloc(sizeof(uhci_transfer_descriptor));
	uhci_transfer_descriptor* td_out = (uhci_transfer_descriptor*) malloc(sizeof(uhci_transfer_descriptor));
	uhci_transfer_descriptor* td_in = (uhci_transfer_descriptor*) malloc(sizeof(uhci_transfer_descriptor));
	uint32_t* uhci_qh_ptr = (uint32_t*) malloc(16);

	uint32_t uhci_request = (uint32_t) malloc(8);
	uint32_t uhci_response = (uint32_t) malloc(0x22); 
	
	



	// SETUP TD
	td_0->next_descriptor = ((uint32_t)(uint64_t)td_1) | 0x4; // or on depth bit
	td_0->status.low_speed = 0;
	td_0->status.active = 1;
	td_0->status.error_counter = 3; 
	td_0->packet_header.packet_type = 0x2d;
	td_0->packet_header.device = 0b0000000;
	td_0->packet_header.endpoint = 0x0;
	td_0->packet_header.data_toggle = 0b0;
	td_0->packet_header.reserved = 0b0;
	td_0->packet_header.maximum_length = 7;
	td_0->buffer_addr = uhci_request;

	// IN TD's
	td_1->next_descriptor = ((uint32_t)(uint64_t)td_out) | 0x4;
	td_1->status.low_speed = 0;
	td_1->status.active = 1;
	td_1->status.error_counter = 3; 
	td_1->packet_header.packet_type = 0x69;	
	td_1->packet_header.device = 0b0000000; // Now we talk to the device we set up as device 1
	td_1->packet_header.endpoint = 0x0;
	td_1->packet_header.data_toggle = 0b1;
	td_1->packet_header.reserved = 0b0;
	td_1->packet_header.maximum_length = 7;
	td_1->buffer_addr = uhci_response;

	//OUT TD
	td_out->next_descriptor = 0x00000001;
	td_out->status.low_speed = 0;
	td_out->status.active = 1;
	td_out->status.error_counter = 3;
	td_out->packet_header.packet_type = 0xe1;	
	td_out->packet_header.device = 0b0000000; // Now we talk to the device we set up as device 1
	td_out->packet_header.endpoint = 0x0;
	td_out->packet_header.data_toggle = 0b1;
	td_out->packet_header.reserved = 0b0;
	td_out->packet_header.maximum_length = 2047; //63 //2047
	td_out->buffer_addr = 0x0;

	// UHCI REQUEST
	usb_device_request* get_descriptor = (usb_device_request*)(uint64_t)(uhci_request);
	get_descriptor->request_type.transfer_direction = usb_transfer_directions.DEVICE_TO_HOST;
	get_descriptor->request_type.request_type = usb_request_types.STANDARD;
	get_descriptor->request_type.recipient = usb_recipients.DEVICE;
	get_descriptor->b_request = usb_standard_requests.GET_DESCRIPTOR;
	get_descriptor->w_value = (b_descriptor_types.DEVICE << 8) | 0x00;
	get_descriptor->w_index = 0x0000;
	get_descriptor->w_length = (0x00 << 8) | 0x08; //b_descriptor_sizes.DEVICE;

	// QH	
	*uhci_qh_ptr = (uint32_t)(uint64_t)td_out;// horizontal pointer to terminate // (uint32_t)(uint64_t)td_out; //horizontal pointer to td_out??? // 0x00000001
	*(uhci_qh_ptr+1) = (uint32_t)(uint64_t)td_0; // vertical pointer to td_0
	*(uhci_qh_ptr+2) = 0x00000000;
	*(uhci_qh_ptr+3) = 0x00000000;
	
	//sleep(50);

	// Add new QH to frame list
	*(uhci_frame_list_ptr) = ((uint32_t)(uint64_t)uhci_qh_ptr) | 0x2; // ptr to qh plus set as qh 
	
	print_string("\n\r");
	print_string("Transfer Setup Completed");


	uint32_t loops = 0;
	while (1) {
		if ( td_out->status.active == 0b0) {
			// On Success
			if ( (td_1->status.actual_length == 7)  ) {
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
	
	print_string("\n\r");
	print_string("[QH]");
	print_qh(uhci_qh_ptr);


	print_string("\n\r");
	print_string("\n\r");
	print_string("[TD_0] ");
	print_td((uint32_t*)td_0);

	print_string("\n\r");
	print_string("\n\r");
	print_string("[TD_1] ");
	print_td((uint32_t*)td_1);

	print_string("\n\r");
	print_string("\n\r");
	print_string("[TD_out] ");
	print_td((uint32_t*)td_out);

	print_string("\n\r");
	print_string("\n\r");
	print_string("RESPONSE");
	print_string("\n\r");
	print_string(hex_to_str(*(uint32_t*)uhci_response));
	print_string("\n\r");
	print_string(hex_to_str(*(((uint32_t*)(uhci_response))+1)));
	
	// SET ADDRESS
	td_0->next_descriptor = ((uint32_t)(uint64_t)td_in) | 0x4; // or on depth bit
	td_0->status.low_speed = 0;
	td_0->status.active = 1;
	td_0->status.error_counter = 3; 
	td_0->packet_header.packet_type = 0x2d;
	td_0->packet_header.device = 0b0000000;
	td_0->packet_header.endpoint = 0x0;
	td_0->packet_header.data_toggle = 0b0;
	td_0->packet_header.reserved = 0b0;
	td_0->packet_header.maximum_length = 7;
	td_0->buffer_addr = uhci_request;

	// IN TD
	td_in->next_descriptor = 0x1; // last td in this transfer
	td_in->status.low_speed = 0;
	td_in->status.active = 1; 
	td_in->status.error_counter = 3; 
	td_in->packet_header.packet_type = 0x69;//td_1->packet_header = (uhci_descriptor_packet_header)0xFFE80069; 
	td_in->packet_header.device = 0b0000000;
	td_in->packet_header.endpoint = 0x0;
	td_in->packet_header.data_toggle = 0b1;
	td_in->packet_header.reserved = 0b0;
	td_in->packet_header.maximum_length = 2047; // all 11 bits as 1's = 0 byte response
	td_in->buffer_addr = uhci_response ; //0x0;

	// UHCI REQUEST
	get_descriptor->request_type.transfer_direction = usb_transfer_directions.HOST_TO_DEVICE;
	get_descriptor->request_type.request_type = usb_request_types.STANDARD;
	get_descriptor->request_type.recipient = usb_recipients.DEVICE;
	get_descriptor->b_request = usb_standard_requests.SET_ADDRESS;
	get_descriptor->w_value = 0x0001;
	get_descriptor->w_index = 0x0000;
	get_descriptor->w_length = (0x00 << 8) | 0x00; //b_descriptor_sizes.DEVICE;

	//UHCI RESPONSE 
	uint32_t* tmp_response = (uint32_t*)uhci_response;
	*tmp_response = 0x00000000;
	*(tmp_response+1) = 0x00000000;

	// QH
	*uhci_qh_ptr = (uint32_t)(uint64_t)td_in; //horizontal pointer to td_out 
	*(uhci_qh_ptr+1) = (uint32_t)(uint64_t)td_0; // vertical pointer to td_0

	// Add new QH to frame list
	*(uhci_frame_list_ptr) = ((uint32_t)(uint64_t)uhci_qh_ptr) | 0x2; // ptr to qh plus set as qh 


	print_string("\n\r");
	print_string("Transfer Setup Completed");


	loops = 0;
	while (1) {
		if ( td_in->status.active == 0b0) {
			// On Success
			if ( (td_0->status.actual_length == 7)  ) {
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




	print_string("\n\r");
	print_string("\n\r");
	print_string("[REQUEST] ");
	print_string("@");
	print_string(hex_to_str(get_descriptor));
	print_string("\n\r");
	uint32_t* tmp_request = (uint32_t*) get_descriptor;
	print_string(hex_to_str(*(tmp_request)));
	print_string("\n\r");
	print_string(hex_to_str(*(tmp_request+1)));
	print_string("\n\r");
	print_string("\n\r");

	print_string("\n\r");
	print_string("[QH]");
	print_qh(uhci_qh_ptr);


	print_string("\n\r");
	print_string("\n\r");
	print_string("[TD_0] ");
	print_td((uint32_t*)td_0);


	print_string("\n\r");
	print_string("\n\r");
	print_string("[TD_IN] ");
	print_td((uint32_t*)td_in);

	print_string("\n\r");
	print_string("\n\r");
	print_string("RESPONSE");
	print_string("\n\r");
	print_string(hex_to_str(*(uint32_t*)uhci_response));
	print_string("\n\r");
	print_string(hex_to_str(*(((uint32_t*)(uhci_response))+1)));

	// SETUP TD
	td_0->next_descriptor = ((uint32_t)(uint64_t)td_1) | 0x4; // or on depth bit
	td_0->status.low_speed = 0;
	td_0->status.active = 1;
	td_0->status.error_counter = 3; 
	td_0->packet_header.packet_type = 0x2d;
	td_0->packet_header.device = 0b0000001;
	td_0->packet_header.endpoint = 0x0;
	td_0->packet_header.data_toggle = 0b0;
	td_0->packet_header.reserved = 0b0;
	td_0->packet_header.maximum_length = 7;
	td_0->buffer_addr = uhci_request;

	// IN TD's
	td_1->next_descriptor = ((uint32_t)(uint64_t)td_2) | 0x4;
	td_1->status.low_speed = 0;
	td_1->status.active = 1;
	td_1->status.error_counter = 3; 
	td_1->packet_header.packet_type = 0x69;	
	td_1->packet_header.device = 0b0000001; // Now we talk to the device we set up as device 1
	td_1->packet_header.endpoint = 0x0;
	td_1->packet_header.data_toggle = 0b1;
	td_1->packet_header.reserved = 0b0;
	td_1->packet_header.maximum_length = 7;
	td_1->buffer_addr = uhci_response;

		// IN TD's
	td_2->next_descriptor = ((uint32_t)(uint64_t)td_3) | 0x4;
	td_2->status.low_speed = 0;
	td_2->status.active = 1;
	td_2->status.error_counter = 3; 
	td_2->packet_header.packet_type = 0x69;	
	td_2->packet_header.device = 0b0000001; // Now we talk to the device we set up as device 1
	td_2->packet_header.endpoint = 0x0;
	td_2->packet_header.data_toggle = 0b0;
	td_2->packet_header.reserved = 0b0;
	td_2->packet_header.maximum_length = 7;
	td_2->buffer_addr = uhci_response+8;

		// IN TD's
	td_3->next_descriptor = ((uint32_t)(uint64_t)td_out) | 0x4;
	td_3->status.low_speed = 0;
	td_3->status.active = 1;
	td_3->status.error_counter = 3; 
	td_3->packet_header.packet_type = 0x69;	
	td_3->packet_header.device = 0b0000001; // Now we talk to the device we set up as device 1
	td_3->packet_header.endpoint = 0x0;
	td_3->packet_header.data_toggle = 0b1;
	td_3->packet_header.reserved = 0b0;
	td_3->packet_header.maximum_length = 7;
	td_3->buffer_addr = uhci_response+16;

	//OUT TD
	td_out->next_descriptor = 0x00000001;
	td_out->status.low_speed = 0;
	td_out->status.active = 1;
	td_out->status.error_counter = 3;
	td_out->packet_header.packet_type = 0xe1;	
	td_out->packet_header.device = 0b0000001; // Now we talk to the device we set up as device 1
	td_out->packet_header.endpoint = 0x0;
	td_out->packet_header.data_toggle = 0b1;
	td_out->packet_header.reserved = 0b0;
	td_out->packet_header.maximum_length = 2047; //63 //2047
	td_out->buffer_addr = 0x0;

	// UHCI REQUEST
	get_descriptor->request_type.transfer_direction = usb_transfer_directions.DEVICE_TO_HOST;
	get_descriptor->request_type.request_type = usb_request_types.STANDARD;
	get_descriptor->request_type.recipient = usb_recipients.DEVICE;
	get_descriptor->b_request = usb_standard_requests.GET_DESCRIPTOR;
	get_descriptor->w_value = (b_descriptor_types.DEVICE << 8) | 0x00;
	get_descriptor->w_index = 0x0000;
	get_descriptor->w_length = (0x00 << 8) | b_descriptor_sizes.DEVICE;

	// QH	
	*uhci_qh_ptr = (uint32_t)(uint64_t)td_out;// horizontal pointer to terminate // (uint32_t)(uint64_t)td_out; //horizontal pointer to td_out??? // 0x00000001
	*(uhci_qh_ptr+1) = (uint32_t)(uint64_t)td_0; // vertical pointer to td_0
	*(uhci_qh_ptr+2) = 0x00000000;
	*(uhci_qh_ptr+3) = 0x00000000;
	
	//sleep(50);

	// Add new QH to frame list
	*(uhci_frame_list_ptr) = ((uint32_t)(uint64_t)uhci_qh_ptr) | 0x2; // ptr to qh plus set as qh 

	print_string("\n\r");
	print_string("Transfer Setup Completed");


	loops = 0;
	while (1) {
		if ( td_out->status.active == 0b0) {
			// On Success
			if ( (td_0->status.actual_length == 7)  ) {
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



	
	
	print_string("\n\r");
	print_string("\n\r");
	print_string("[REQUEST] ");
	print_string("@");
	print_string(hex_to_str(get_descriptor));
	print_string("\n\r");
	//uint32_t* tmp_request = (uint32_t*) get_descriptor;
	print_string(hex_to_str(*(tmp_request)));
	print_string("\n\r");
	print_string(hex_to_str(*(tmp_request+1)));
	print_string("\n\r");
	print_string("\n\r");

	print_string("\n\r");
	print_string("[QH]");
	print_qh(uhci_qh_ptr);


	print_string("\n\r");
	print_string("\n\r");
	print_string("[TD_0] ");
	print_td((uint32_t*)td_0);

	print_string("\n\r");
	print_string("\n\r");
	print_string("[TD_1] ");
	print_td((uint32_t*)td_1);

	print_string("\n\r");
	print_string("\n\r");
	print_string("[TD_2] ");
	print_td((uint32_t*)td_2);

	print_string("\n\r");
	print_string("\n\r");
	print_string("[TD_3] ");
	print_td((uint32_t*)td_3);

	print_string("\n\r");
	print_string("\n\r");
	print_string("[TD_OUT] ");
	print_td((uint32_t*)td_out);

	print_string("\n\r");
	print_string("\n\r");
	print_string("RESPONSE");
	print_string("\n\r");
	usb_descriptor_device* descriptor_device = (usb_descriptor_device*) uhci_response;
	print_string(hex_to_str(*(uint32_t*)uhci_response));
	print_string("\n\r");
	print_string(hex_to_str(*(((uint32_t*)(uhci_response))+1)));
	print_string("\n\r");
	print_string(hex_to_str(*(((uint32_t*)(uhci_response))+2)));
	print_string("\n\r");
	print_string(hex_to_str(*(((uint32_t*)(uhci_response))+3)));
	print_string("\n\r");
	print_string(hex_to_str(*(((uint32_t*)(uhci_response))+4)));
	
	
	// SETUP TD
	td_0->next_descriptor = ((uint32_t)(uint64_t)td_1) | 0x4; // or on depth bit
	td_0->status.low_speed = 0;
	td_0->status.active = 1;
	td_0->status.error_counter = 3; 
	td_0->packet_header.packet_type = 0x2d;
	td_0->packet_header.device = 0b0000001;
	td_0->packet_header.endpoint = 0x0;
	td_0->packet_header.data_toggle = 0b0;
	td_0->packet_header.reserved = 0b0;
	td_0->packet_header.maximum_length = 7;
	td_0->buffer_addr = uhci_request;

	// IN TD's
	td_1->next_descriptor = ((uint32_t)(uint64_t)td_2) | 0x4;
	td_1->status.low_speed = 0;
	td_1->status.active = 1;
	td_1->status.error_counter = 3; 
	td_1->packet_header.packet_type = 0x69;	
	td_1->packet_header.device = 0b0000001; // Now we talk to the device we set up as device 1
	td_1->packet_header.endpoint = 0x0;
	td_1->packet_header.data_toggle = 0b1;
	td_1->packet_header.reserved = 0b0;
	td_1->packet_header.maximum_length = 7;
	td_1->buffer_addr = uhci_response;

	
	// IN TD's
	td_2->next_descriptor = ((uint32_t)(uint64_t)td_3) | 0x4;
	td_2->status.low_speed = 0;
	td_2->status.active = 1;
	td_2->status.error_counter = 3; 
	td_2->packet_header.packet_type = 0x69;	
	td_2->packet_header.device = 0b0000001; // Now we talk to the device we set up as device 1
	td_2->packet_header.endpoint = 0x0;
	td_2->packet_header.data_toggle = 0b0;
	td_2->packet_header.reserved = 0b0;
	td_2->packet_header.maximum_length = 7;
	td_2->buffer_addr = uhci_response+8;

	
		// IN TD's
	td_3->next_descriptor = ((uint32_t)(uint64_t)td_4) | 0x4;
	td_3->status.low_speed = 0;
	td_3->status.active = 1;
	td_3->status.error_counter = 3; 
	td_3->packet_header.packet_type = 0x69;	
	td_3->packet_header.device = 0b0000001; // Now we talk to the device we set up as device 1
	td_3->packet_header.endpoint = 0x0;
	td_3->packet_header.data_toggle = 0b1;
	td_3->packet_header.reserved = 0b0;
	td_3->packet_header.maximum_length = 7;
	td_3->buffer_addr = uhci_response+16;

	// IN TD's
	td_4->next_descriptor = ((uint32_t)(uint64_t)td_out) | 0x4;
	td_4->status.low_speed = 0;
	td_4->status.active = 1;
	td_4->status.error_counter = 3; 
	td_4->packet_header.packet_type = 0x69;	
	td_4->packet_header.device = 0b0000001; // Now we talk to the device we set up as device 1
	td_4->packet_header.endpoint = 0x0;
	td_4->packet_header.data_toggle = 0b1;
	td_4->packet_header.reserved = 0b0;
	td_4->packet_header.maximum_length = 7;
	td_4->buffer_addr = uhci_response+24;
	

	//OUT TD
	td_out->next_descriptor = 0x00000001;
	td_out->status.low_speed = 0;
	td_out->status.active = 1;
	td_out->status.error_counter = 3;
	td_out->packet_header.packet_type = 0xe1;	
	td_out->packet_header.device = 0b0000001; // Now we talk to the device we set up as device 1
	td_out->packet_header.endpoint = 0x0;
	td_out->packet_header.data_toggle = 0b1;
	td_out->packet_header.reserved = 0b0;
	td_out->packet_header.maximum_length = 2047; //63 //2047
	td_out->buffer_addr = 0x0;

	// UHCI REQUEST
	get_descriptor->request_type.transfer_direction = usb_transfer_directions.DEVICE_TO_HOST;
	get_descriptor->request_type.request_type = usb_request_types.STANDARD;
	get_descriptor->request_type.recipient = usb_recipients.DEVICE;
	get_descriptor->b_request = usb_standard_requests.GET_DESCRIPTOR;
	get_descriptor->w_value = (b_descriptor_types.STRING << 8) | 0x02; //
	get_descriptor->w_index = usb_language_id.ENGLISH_US;
	get_descriptor->w_length = (0x00 << 8) | 0x1e;

	//UHCI RESPONSE 
	//uint32_t* tmp_response = (uint32_t*)uhci_response;
	*tmp_response = 0x00000000;
	*(tmp_response+1) = 0x00000000;
	*(tmp_response+2) = 0x00000000;
	*(tmp_response+3) = 0x00000000;
	*(tmp_response+4) = 0x00000000;
	*(tmp_response+5) = 0x00000000;

	// QH	
	*uhci_qh_ptr = (uint32_t)(uint64_t)td_out;// horizontal pointer to terminate // (uint32_t)(uint64_t)td_out; //horizontal pointer to td_out??? // 0x00000001
	*(uhci_qh_ptr+1) = (uint32_t)(uint64_t)td_0; // vertical pointer to td_0
	*(uhci_qh_ptr+2) = 0x00000000;
	*(uhci_qh_ptr+3) = 0x00000000;
	
	//sleep(50);

	// Add new QH to frame list
	*(uhci_frame_list_ptr) = ((uint32_t)(uint64_t)uhci_qh_ptr) | 0x2; // ptr to qh plus set as qh 

	print_string("\n\r");
	print_string("Transfer Setup Completed");


	loops = 0;
	while (1) {
		if ( td_out->status.active == 0b0) {
			// On Success
			if ( (td_0->status.actual_length == 7)  ) {
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
	
	print_string("\n\r");
	print_string("\n\r");
	print_string("[REQUEST] ");
	print_string("@");
	print_string(hex_to_str(get_descriptor));
	print_string("\n\r");
	//uint32_t* tmp_request = (uint32_t*) get_descriptor;
	print_string(hex_to_str(*(tmp_request)));
	print_string("\n\r");
	print_string(hex_to_str(*(tmp_request+1)));
	print_string("\n\r");
	print_string("\n\r");

	print_string("\n\r");
	print_string("[QH]");
	print_qh(uhci_qh_ptr);


	print_string("\n\r");
	print_string("\n\r");
	print_string("[TD_0] ");
	print_td((uint32_t*)td_0);

	print_string("\n\r");
	print_string("\n\r");
	print_string("[TD_1] ");
	print_td((uint32_t*)td_1);

	
	print_string("\n\r");
	print_string("\n\r");
	print_string("[TD_2] ");
	print_td((uint32_t*)td_2);

	print_string("\n\r");
	print_string("\n\r");
	print_string("[TD_3] ");
	print_td((uint32_t*)td_3);

	print_string("\n\r");
	print_string("\n\r");
	print_string("[TD_4] ");
	print_td((uint32_t*)td_4);
	

	print_string("\n\r");
	print_string("\n\r");
	print_string("[TD_OUT] ");
	print_td((uint32_t*)td_out);

	print_string("\n\r");
	print_string("\n\r");
	print_string("RESPONSE");
	print_string("\n\r");
	//usb_descriptor_device* descriptor_device = (usb_descriptor_device*) uhci_response;
	print_string(hex_to_str(*(uint32_t*)uhci_response));
	print_string("\n\r");
	print_string(hex_to_str(*(((uint32_t*)(uhci_response))+1)));
	print_string("\n\r");
	print_string(hex_to_str(*(((uint32_t*)(uhci_response))+2)));
	print_string("\n\r");
	print_string(hex_to_str(*(((uint32_t*)(uhci_response))+3)));
	print_string("\n\r");
	print_string(hex_to_str(*(((uint32_t*)(uhci_response))+4)));
	print_string("\n\r");
	print_string(hex_to_str(*(((uint32_t*)(uhci_response))+5)));

	print_string("\n\r");
	print_string(hex_to_str(*(((uint32_t*)(uhci_response))+6)));
	print_string("\n\r");
	print_string(hex_to_str(*(((uint32_t*)(uhci_response))+7)));
	
	
	
	print_string("\n\r");
	print_string("\n\r");
	int uhci_sz = 0x1e;
	for (int i=2; i<uhci_sz; i++) {
		uint8_t* response_ptr = (uint8_t*) uhci_response;
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

uint8_t uhci_set_addr () {

}

uint8_t uhci_get_descriptor () {

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


