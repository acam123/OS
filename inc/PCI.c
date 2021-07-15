#include "PCI.h"


void init_pci() {
	if ( pci_check == PCI_MAGIC) {
		//print_string("PCI Magic Found!!!");
	}
	else {
		//print_string("NO PCI");
	}

	brute_force_devs();

}


void brute_force_devs() {
	uint16_t bus;
	uint8_t dev;

	uint16_t vendor_id;
	uint8_t header_type;

	for(bus = 0; bus < 256; bus++) {
		for(dev = 0; dev < 32; dev++) {
			vendor_id = get_vendor_id(bus, dev, 0);
			if (vendor_id != 0xffff) {
				header_type = get_header_type(bus, dev, 0);
				print_string("\n\rBus:");
				print_string(int_to_str(bus));
				print_string("\n\rDevice:");
				print_string(int_to_str(dev));
				print_string("\n\rHeader:");
				print_string(hex_to_str(header_type));
				// check for multi function
				if ( (header_type & 0x80) != 0) {
					uint16_t device_id;
					print_string("\n\r-- func_0: ");
					print_string(hex_to_str(vendor_id));
					device_id = get_device_id(bus, dev, 0);
					print_string(" -> ");
					print_string(hex_to_str(device_id));

					header_type = get_header_type(bus, dev, 0);
					print_string("; ");
					print_string(hex_to_str(header_type));

					for (uint8_t i = 1; i < 8; i++ ) {
						vendor_id = get_vendor_id(bus, dev, i);
						print_string("\n\r-- func_");
						print_string(int_to_str(i));
						print_string(": ");
						print_string(hex_to_str(vendor_id));
						if (vendor_id != 0xffff) {
							device_id = get_device_id(bus, dev, i);
							print_string(" -> ");
							print_string(hex_to_str(device_id));

							header_type = get_header_type(bus, dev, i);
							print_string("; ");
							print_string(hex_to_str(header_type));

						}

					}
				}
				else {
					uint16_t device_id;
					print_string("\n\r-- func_0: ");
					print_string(hex_to_str(vendor_id));
					device_id = get_device_id(bus, dev, 0);
					print_string(" -> ");
					print_string(hex_to_str(device_id));

					header_type = get_header_type(bus, dev, 0);
					print_string("; ");
					print_string(hex_to_str(header_type));

					if(device_id == 0x24cd) {
						uint8_t class_code = get_class_code(bus, dev, 0);
						uint8_t subclass = get_subclass_code(bus, dev, 0);
						uint8_t prog_if = get_prog_if(bus, dev, 0);
						uint16_t subsys_vendor_id = get_subsys_vendor_id(bus, dev, 0);
						uint16_t subsys_id = get_subsys_id(bus, dev, 0);

						print_string(" :: ");
						print_string(hex_to_str(class_code));
						print_string(", ");
						print_string(hex_to_str(subclass));
						print_string(", ");
						print_string(hex_to_str(prog_if));
						print_string(", ");
						print_string(hex_to_str(subsys_vendor_id));
						print_string(", ");
						print_string(hex_to_str(subsys_id));


					}


				}
			}
		}
	}
}

uint16_t get_device_id(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = query_pci (bus, dev, func, 0);
	ret = (ret & 0xffff0000) >> 16;
	return (uint16_t)ret;
}

uint16_t get_vendor_id(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = query_pci (bus, dev, func, 0);
	ret &= 0x0000ffff;
	return (uint16_t)ret;
}

uint16_t get_status(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = query_pci (bus, dev, func, 1);
	ret = (ret & 0xffff0000) >> 16;
	return (uint16_t)ret;
}

uint16_t get_command (uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = query_pci (bus, dev, func, 1);
	ret &= 0x0000ffff;
	return (uint16_t)ret;
}


uint8_t get_class_code(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = query_pci (bus, dev, func, 2);
	ret = (ret & 0xff000000) >> 24;
	return (uint8_t)ret;
}

uint8_t get_subclass_code(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = query_pci (bus, dev, func, 2);
	ret = (ret & 0x00ff0000) >> 16;
	return (uint8_t)ret;
}

uint8_t get_prog_if(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = query_pci (bus, dev, func, 2);
	ret = (ret & 0x0000ff00) >> 8;
	return (uint8_t)ret;
}

uint8_t get_revision_id(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = query_pci (bus, dev, func, 2);
	ret &= 0x000000ff;
	return (uint8_t)ret;
}

uint8_t get_bist(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = query_pci (bus, dev, func, 3);
	ret = (ret & 0xff000000) >> 24;
	return (uint8_t)ret;
}

uint8_t get_header_type(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = query_pci (bus, dev, func, 3);
	ret = (ret & 0x00ff0000) >> 16;
	return (uint8_t)ret;
}

uint8_t get_latency_timer(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = query_pci (bus, dev, func, 3);
	ret = (ret & 0x0000ff00) >> 8;
	return (uint8_t)ret;
}

uint8_t get_cache_line_sz(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = query_pci (bus, dev, func, 3);
	ret &= 0x000000ff;
	return (uint8_t)ret;
}

uint32_t get_bar_0(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = query_pci (bus, dev, func, 4);
	return ret;
}

uint32_t get_bar_1(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = query_pci (bus, dev, func, 5);
	return ret;
}

uint32_t get_bar_2(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = query_pci (bus, dev, func, 6);
	return ret;
}

uint32_t get_bar_3(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = query_pci (bus, dev, func, 7);
	return ret;
}

uint32_t get_bar_4(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = query_pci (bus, dev, func, 8);
	return ret;
}

uint32_t get_bar_5(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = query_pci (bus, dev, func, 9);
	return ret;
}

uint32_t get_cardbus_cis_ptr(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = query_pci (bus, dev, func, 0xa);
	return ret;
}

uint16_t get_subsys_id(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = query_pci (bus, dev, func, 0xb);
	ret = (ret & 0xffff0000) >> 16;
	return (uint16_t)ret;
}

uint16_t get_subsys_vendor_id(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = query_pci (bus, dev, func, 0xb);
	ret &= 0x0000ffff;
	return (uint16_t)ret;
}

uint32_t get_expansion_rom_addr(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = query_pci (bus, dev, func, 0xc);
	return ret;
}

uint8_t get_capabilities_ptr(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = query_pci (bus, dev, func, 0xd);
	ret &= 0x000000ff;
	return (uint8_t)ret;
}

uint8_t get_max_latency(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = query_pci (bus, dev, func, 0xf);
	ret = (ret & 0xff000000) >> 24;
	return (uint8_t)ret;
}

uint8_t get_min_grant(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = query_pci (bus, dev, func, 0xf);
	ret = (ret & 0x00ff0000) >> 16;
	return (uint8_t)ret;
}

uint8_t get_interrupt_pin(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = query_pci (bus, dev, func, 0xf);
	ret = (ret & 0x0000ff00) >> 8;
	return (uint8_t)ret;
}

uint8_t get_interrupt_line(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = query_pci (bus, dev, func, 0xf);
	ret &= 0x000000ff;
	return (uint8_t)ret;
}



uint32_t query_pci (uint8_t bus, uint8_t dev, uint8_t func, uint8_t reg) {
	pci_dev_t pci;
	pci.query.dev_num = dev;
	pci.query.bus_num = bus;
	pci.query.func_num = func;
	pci.query.reg_num = reg;

	pci.query.always_zero = 0b00;
	pci.query.reserved = 0b0000000;
	pci.query.enable = 0b1; 
	outl(PCI_CONFIG_ADDRESS, pci.bits);
	uint32_t ret = inl(PCI_CONFIG_DATA);
	return ret;
}
/*

 void checkDevice(uint8_t bus, uint8_t device) {
     uint8_t function = 0;
 
     vendor_id = get_vendor_id(bus, device, function);
     if(vendor_id == 0xFFFF) return;        // Device doesn't exist
     checkFunction(bus, device, function);
     header_type = get_header_type(bus, device, function);
     if( (header_type & 0x80) != 0) {
         // It is a multi-function device, so check remaining functions 
         for(function = 1; function < 8; function++) {
             if(get_vendor_id(bus, device, function) != 0xFFFF) {
                 checkFunction(bus, device, function);
             }
         }
     }
 }




*/