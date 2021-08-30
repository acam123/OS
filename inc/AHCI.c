#include "AHCI.h"

hba_port_tracker_t hba_port_tracker[32];
uint8_t hba_port_tracker_count; 

void init_ahci() {
	// this should be a list of ahci endpoints instead
	uint32_t ahci_base_addr = get_bar_5(ahci_endpoints[0].bus, ahci_endpoints[0].dev, ahci_endpoints[0].func);
	map_memory(ahci_base_addr, ahci_base_addr, (page_table*) 0x1000);
	hba_port_tracker_count = 0;
	
	probe_ports(ahci_base_addr);
	
	
	configure_port(hba_port_tracker[0].hba_port_ptr);
	
	
	uint8_t* buffer = (uint8_t*) request_page();
	memset(0, (uint64_t*)buffer, 0x1000);

	uint8_t res = disk_read(0, 4, (void*)buffer, hba_port_tracker[0].hba_port_ptr);  //hba_port* hba_port_ptr
	
	/*
	print_string("\n\rTRACKER =>");
	print_string(" type:");
	print_string(hex_to_str(hba_port_tracker[0].port_type));
	print_string(" :: addr:");
	print_string(hex_to_str((uint64_t)hba_port_tracker[0].hba_port_ptr));
	print_string(" :: count:");
	print_string(hex_to_str((uint64_t)hba_port_tracker[0].port_num));
	*/
	
	/*
	print_string("\n\rRead Result:");
	print_string(hex_to_str(res));
	for (int t=0; t<1024; t++) {
		print_char(buffer[t]); //buffer[t]);	
		//print_string(hex_to_str(buffer[t]));	
	}
	*/

}

void probe_ports(uint32_t ahci_base_addr) {
	hba_memory* hba_memory_ptr = (hba_memory*)(uint64_t)ahci_base_addr;
	// ports_implemented is a preset bitmap of the 32 ports
	uint32_t ports_implemented = hba_memory_ptr->ports_implemented;
	for (int i=0; i<32; i++) {
		if (ports_implemented & (1 << i)) {
			hba_port* hba_port_ptr = (hba_port*)&(hba_memory_ptr->ports[i]);
			ahci_port_type port_type = get_port_type(hba_port_ptr);
			if (port_type == sata || port_type == satapi ) {
				hba_port_tracker[hba_port_tracker_count].port_type = port_type; 
				hba_port_tracker[hba_port_tracker_count].hba_port_ptr = hba_port_ptr;
				hba_port_tracker[hba_port_tracker_count].port_num = hba_port_tracker_count;
				
				print_string("\n\r");
				print_string("--- Ports ---");
				print_string("\n\r");
				print_string("Type:");
				print_string(hex_to_str(port_type));
				print_string("\n\r");
				print_string("Count:");
				print_string(hex_to_str(hba_port_tracker_count));
				print_string("\n\r");
				print_string("Addr:");
				print_string(hex_to_str((uint64_t)hba_port_ptr));

				hba_port_tracker_count++;
			}
		}
	}
}

ahci_port_type get_port_type( hba_port* port) {
	uint32_t sata_status = port->sata_status;
	uint8_t device_detection = sata_status & 0b1111;
	uint8_t interface_power_management = (sata_status >> 8) & 0b1111;
	ahci_port_type port_type;

	if (device_detection != HBA_PORT_DEV_PRESENT) {
		port_type = none;
		return port_type;
	}

	if (interface_power_management != HBA_PORT_IPM_ACTIVE) {
		port_type = none;
		return port_type;
	}

	switch (port->signature) {
		case SATA_SIG_ATAPI:
			port_type = satapi;
		    break;
		case SATA_SIG_ATA:
		    port_type = sata;
		    break;
		case SATA_SIG_PM:
		   port_type = pm;
		   break;
		case SATA_SIG_SEMB:
		    port_type = semb;
		    break;
		default:
		   port_type = none;
	}
	return port_type;
}

void configure_port (hba_port* hba_port_ptr) {
	port_stop_command(hba_port_ptr);

	//rebase
	uint64_t command_list_base = (uint64_t) request_page();
	hba_port_ptr->command_list_base_addr_lower = (uint32_t) (command_list_base /*& 0x00000000ffffffff*/);
	hba_port_ptr->command_list_base_addr_upper = (uint32_t) ((command_list_base /*& 0xffffffff00000000*/) >> 32);
	memset(0, (uint64_t*)(uint64_t)(hba_port_ptr->command_list_base_addr_lower), 1024);


	// maybe just malloc 256 bytes;
	uint64_t fis_base = (uint64_t) request_page();
	hba_port_ptr->fis_base_addr_lower = (uint32_t)(fis_base /*& 0x00000000ffffffff*/);
	hba_port_ptr->fis_base_addr_upper = (uint32_t)((fis_base /*& 0xffffffff00000000*/) >> 32);
	memset(0, (uint64_t*)(uint64_t)(hba_port_ptr->fis_base_addr_lower), 256);

	hba_command_header* command_header_ptr = (hba_command_header*) (((uint64_t)hba_port_ptr->command_list_base_addr_upper << 32) + (uint64_t)hba_port_ptr->command_list_base_addr_lower);

	for (int i=0; i<32; i++) {
		command_header_ptr[i].phys_region_descriptor_table_length = 8;
		uint64_t command_table_addr = (uint64_t) request_page();
		uint64_t addr = command_table_addr + (i << 8);

		command_header_ptr[i].command_table_base_addr_lower = (uint32_t) (addr /*& 0x00000000ffffffff*/);
		command_header_ptr[i].command_table_base_addr_upper = (uint32_t) ((addr /*& 0xffffffff00000000*/) >> 32);
		memset(0, (uint64_t*)command_table_addr, 256);

	}

	port_start_command(hba_port_ptr);
}

void port_stop_command (hba_port* hba_port_ptr) {
	hba_port_ptr->command_status &= ~HBA_PxCMD_ST;
	hba_port_ptr->command_status &= ~HBA_PxCMD_FRE;

	while (1) {
		if ( !(hba_port_ptr->command_status & HBA_PxCMD_FR) ) {
			break;
		}
		if ( !(hba_port_ptr->command_status & HBA_PxCMD_CR) ) {
			break;
		}	
	}
}

void port_start_command (hba_port* hba_port_ptr) {
	while (hba_port_ptr->command_status & HBA_PxCMD_CR);
	hba_port_ptr->command_status |= HBA_PxCMD_FRE;
	hba_port_ptr->command_status |= HBA_PxCMD_ST;
}

uint8_t disk_read(uint64_t sector, uint32_t sector_count, void* buffer, hba_port* hba_port_ptr) {
	uint32_t sector_low = (uint32_t) sector /*&  0x00000000ffffffff*/;
	uint32_t sector_high = (uint32_t) ((sector /*& 0xffffffff00000000*/) >> 32);

	// clear pending interrupt bits 
	hba_port_ptr->interrupt_status = (uint32_t) -1;

	hba_command_header* command_header_ptr = (hba_command_header*)(uint64_t)hba_port_ptr->command_list_base_addr_lower;
	command_header_ptr->command_fis_length = (sizeof(fis_reg_h2d) / sizeof((uint32_t)0)); // command FIS size
	command_header_ptr->write = 0; // read command, no write
	command_header_ptr->phys_region_descriptor_table_length = 1;

	hba_command_table* command_table_ptr = (hba_command_table*)(uint64_t)command_header_ptr->command_table_base_addr_lower;
	memset(0, (uint64_t*)command_table_ptr, sizeof(hba_command_table) + (command_header_ptr->phys_region_descriptor_table_length - 1) * sizeof(hba_prdt_entry)); 

	command_table_ptr->prdt_entry[0].data_base_addr_lower = (uint32_t)((uint64_t)buffer /*& 0x00000000ffffffff*/);
	command_table_ptr->prdt_entry[0].data_base_addr_upper = (uint32_t)(((uint64_t)buffer /*& 0xffffffff00000000*/) >> 32);

	command_table_ptr->prdt_entry[0].byte_count = (sector_count<<9) - 1; // 512 bytes per sector
	command_table_ptr->prdt_entry[0].interrupt_on_completion = 1;

	fis_reg_h2d* command_fis_ptr = (fis_reg_h2d*) &command_table_ptr->command_fis;
	command_fis_ptr->fis_type = fis_type_reg_h2d;
	command_fis_ptr->command_control = 1; //command
	command_fis_ptr->command = ATA_CMD_READ_DMA_EX;

	command_fis_ptr->lba_0 = (uint8_t)(sector_low /*& 0x000000ff*/);
	command_fis_ptr->lba_1 = (uint8_t)((sector_low /*& 0x0000ff00*/) >> 8);
	command_fis_ptr->lba_2 = (uint8_t)((sector_low /*& 0x00ff0000*/) >> 16);
	command_fis_ptr->lba_3 = (uint8_t)(sector_high /*& 0x000000ff*/);
	command_fis_ptr->lba_4 = (uint8_t)((sector_high /*& 0x0000ff00*/) >> 8);
	command_fis_ptr->lba_5 = (uint8_t)((sector_high /*& 0x00ff0000*/) >> 16);

	command_fis_ptr->device_register = LBA_MODE;

	command_fis_ptr->count_low = sector_count & 0xff;
	command_fis_ptr->count_high = (sector_count >> 8) & 0xff;

	// should this go earlier in disk_read
	uint64_t spin = 0;
	while ( (hba_port_ptr->task_file_data & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < 1000000 ) {
		spin++;
	}

	if (spin == 1000000) {
		return 0; // hung port, read failed
	}

	hba_port_ptr->command_issue = 1; //issue the command

	while (1) {
		if ((hba_port_ptr->command_issue == 0)) {
			break;
		}
		if ((hba_port_ptr->interrupt_status & HBA_PxIS_TFES)) {
			return 0; // task file error, read failed
		}
	}
	return 1;
}


