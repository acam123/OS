#include "PCI.h"

pci_meta ahci_endpoints[10];
pci_meta uhci_endpoints[10];
uint8_t ahci_count = 0;
uint8_t uhci_count = 0;

void init_pci() {
	if ( pci_check == PCI_MAGIC) {
		//print_string("PCI Magic Found!!!");
		check_pci();
		print_string("\n\rAHCI_COUNT:");
		print_string(hex_to_str(ahci_count));
		print_string("\n\rUHCI_COUNT:");
		print_string(hex_to_str(uhci_count));
	}
	else {
		//print_string("NO PCI");
	}

}

void check_function (uint8_t bus, uint8_t dev, uint8_t func) {
	uint8_t class_code = get_class_code(bus, dev, func);
	uint8_t subclass = get_subclass(bus, dev, func);
	
	print_string("\n\r");
	print_string("\n\rDev:");
	print_string(hex_to_str(dev));
	print_string(" -> Func:");
	print_string(hex_to_str(func));

	print_string("\n\r -");
	print_string(lookup_class_code(class_code));
	print_string(" :: ");
	print_string(lookup_subclass(class_code, subclass));
	print_string("\n\r -");

	uint16_t vendor_id = get_vendor_id(bus,dev,func);
	print_string(lookup_vendor_id(vendor_id));
	print_string(" :: ");
	uint16_t device_id = get_device_id(bus,dev,func);
	print_string(lookup_device_id(vendor_id, device_id));
	print_string(" :: ");
	uint8_t prog_if = get_prog_if(bus, dev, func);
	print_string(lookup_prog_if(class_code, subclass, prog_if));

	// Initiate AHCI Driver
	if ( class_code == 0x01 && subclass == 0x06 && prog_if == 0x01 ) {
		ahci_endpoints[ahci_count].bus = bus;
		ahci_endpoints[ahci_count].dev = dev;
		ahci_endpoints[ahci_count].func = func; 
		ahci_endpoints[ahci_count].present = 1;
		ahci_count++;
	}

	// Initiate UHCI Driver
	if ( class_code == 0xc && subclass == 0x3 && prog_if == 0x0 ) {
		uhci_endpoints[uhci_count].bus = bus;
		uhci_endpoints[uhci_count].dev = dev;
		uhci_endpoints[uhci_count].func = func;
		uhci_endpoints[uhci_count].present = 1;
		uhci_count++;
	}
	
	// check for PCI-to-PCI Bridge
	if( (class_code == 0x06) && (subclass == 0x04 | subclass == 0x09) ) {
		uint8_t secondary_bus_num = get_secondary_bus_num(bus, dev, func);
		check_bus(secondary_bus_num);
	}
}

void check_device (uint8_t bus, uint8_t dev, uint8_t func) {
	uint16_t vendor_id = get_vendor_id(bus, dev, func);
	if (vendor_id == 0xffff) { 
		return;
	}
	else {
		check_function(bus, dev, func);
     	uint8_t header_type = get_header_type(bus, dev, func);
		if ( (header_type & 0x80) != 0) {	
			// Device is multifunction
			for(func = 1; func < 8; func++) {
				if (get_vendor_id(bus, dev, func) != 0xffff) {
					check_function(bus, dev, func);
				}
			}
		}
	}
}

void check_bus(uint8_t bus) {
	for (uint8_t dev = 0; dev < 32; dev++) {
		check_device(bus, dev, 0);
	}
}

void check_pci () {
	check_bus(0);
}

uint16_t get_device_id(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = pci_read (bus, dev, func, 0);
	ret = (ret & 0xffff0000) >> 16;
	return (uint16_t)ret;
}

uint16_t get_vendor_id(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = pci_read (bus, dev, func, 0);
	ret &= 0x0000ffff;
	return (uint16_t)ret;
}

uint16_t get_status(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = pci_read (bus, dev, func, 1);
	ret = (ret & 0xffff0000) >> 16;
	return (uint16_t)ret;
}

uint16_t get_command (uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = pci_read (bus, dev, func, 1);
	ret &= 0x0000ffff;
	return (uint16_t)ret;
}


uint8_t get_class_code(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = pci_read (bus, dev, func, 2);
	ret = (ret & 0xff000000) >> 24;
	return (uint8_t)ret;
}

uint8_t get_subclass(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = pci_read (bus, dev, func, 2);
	ret = (ret & 0x00ff0000) >> 16;
	return (uint8_t)ret;
}

uint8_t get_prog_if(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = pci_read (bus, dev, func, 2);
	ret = (ret & 0x0000ff00) >> 8;
	return (uint8_t)ret;
}

uint8_t get_revision_id(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = pci_read (bus, dev, func, 2);
	ret &= 0x000000ff;
	return (uint8_t)ret;
}

uint8_t get_bist(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = pci_read (bus, dev, func, 3);
	ret = (ret & 0xff000000) >> 24;
	return (uint8_t)ret;
}

uint8_t get_header_type(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = pci_read (bus, dev, func, 3);
	ret = (ret & 0x00ff0000) >> 16;
	return (uint8_t)ret;
}

uint8_t get_latency_timer(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = pci_read (bus, dev, func, 3);
	ret = (ret & 0x0000ff00) >> 8;
	return (uint8_t)ret;
}

uint8_t get_cache_line_sz(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = pci_read (bus, dev, func, 3);
	ret &= 0x000000ff;
	return (uint8_t)ret;
}

uint32_t get_bar_0(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = pci_read (bus, dev, func, 4);
	return ret;
}

uint32_t get_bar_1(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = pci_read (bus, dev, func, 5);
	return ret;
}

uint32_t get_bar_2(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = pci_read (bus, dev, func, 6);
	return ret;
}

uint32_t get_bar_3(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = pci_read (bus, dev, func, 7);
	return ret;
}

uint32_t get_bar_4(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = pci_read (bus, dev, func, 8);
	return ret;
}

uint32_t get_bar_5(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = pci_read (bus, dev, func, 9);
	return ret;
}

uint32_t get_cardbus_cis_ptr(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = pci_read (bus, dev, func, 0xa);
	return ret;
}

uint16_t get_subsys_id(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = pci_read (bus, dev, func, 0xb);
	ret = (ret & 0xffff0000) >> 16;
	return (uint16_t)ret;
}

uint16_t get_subsys_vendor_id(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = pci_read (bus, dev, func, 0xb);
	ret &= 0x0000ffff;
	return (uint16_t)ret;
}

uint32_t get_expansion_rom_addr(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = pci_read (bus, dev, func, 0xc);
	return ret;
}

uint8_t get_capabilities_ptr(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = pci_read (bus, dev, func, 0xd);
	ret &= 0x000000ff;
	return (uint8_t)ret;
}

uint8_t get_max_latency(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = pci_read (bus, dev, func, 0xf);
	ret = (ret & 0xff000000) >> 24;
	return (uint8_t)ret;
}

uint8_t get_min_grant(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = pci_read (bus, dev, func, 0xf);
	ret = (ret & 0x00ff0000) >> 16;
	return (uint8_t)ret;
}

uint8_t get_interrupt_pin(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = pci_read (bus, dev, func, 0xf);
	ret = (ret & 0x0000ff00) >> 8;
	return (uint8_t)ret;
}

uint8_t get_interrupt_line(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = pci_read (bus, dev, func, 0xf);
	ret &= 0x000000ff;
	return (uint8_t)ret;
}

uint8_t get_secondary_bus_num(uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t ret = pci_read (bus, dev, func, 0x06);
	ret = (ret & 0x0000ff00) >> 8;
	return (uint8_t)ret;
}

uint32_t pci_read (uint8_t bus, uint8_t dev, uint8_t func, uint8_t reg) {
	pci_dev_t pci;
	pci.query.dev_num = dev;
	pci.query.bus_num = bus;
	pci.query.func_num = func;
	pci.query.reg_num = reg;

	pci.query.always_zero = 0b00;
	pci.query.reserved = 0b0000000;
	pci.query.enable = 0b1; 
	out_32(PCI_CONFIG_ADDRESS, pci.bits);
	uint32_t ret = in_32(PCI_CONFIG_DATA);
	return ret;
}

void pci_write (uint8_t bus, uint8_t dev, uint8_t func, uint8_t reg, uint32_t data) {
	pci_dev_t pci;
	pci.query.dev_num = dev;
	pci.query.bus_num = bus;
	pci.query.func_num = func;
	pci.query.reg_num = reg;

	pci.query.always_zero = 0b00;
	pci.query.reserved = 0b0000000;
	pci.query.enable = 0b1; 
	out_32(PCI_CONFIG_ADDRESS, pci.bits);
	out_32(PCI_CONFIG_DATA, data);
	return;
}
