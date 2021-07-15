#ifndef _PCI_ 
#define _PCI_ 

#include "Typedefs.h"
#include "TextPrint.h"
#include "IO.h"

#define PCI_MAGIC 				0x20494350
#define PCI_CONFIG_ADDRESS  	0xCF8
#define PCI_CONFIG_DATA     	0xCFC
#define PCI_MAX_BUS 			256
#define PCI_MAX_DEV 			32
#define PCI_MAX_FUNC 			8
#define PCI_CONFIG_REG_BYTES	256

extern uint32_t pci_check;

// PCI device query structure
typedef struct _pci_dev_query {
	uint32_t always_zero	: 2; 
    uint32_t reg_num		: 6;
    uint32_t func_num		: 3;
    uint32_t dev_num		: 5;
    uint32_t bus_num		: 8;
    uint32_t reserved		: 7;
    uint32_t enable			: 1;
} pci_dev_query;

typedef union _pci_dev_t {
	 uint32_t bits;
	 pci_dev_query query;
} pci_dev_t;

typedef struct _pci_header_type_0 {
	uint16_t vendor_id; 
	uint16_t device_id;

	uint16_t command; 
	uint16_t status;

	uint8_t revision_id; 
	uint8_t prog_if;
	uint8_t subclass;
	uint8_t class_code; 
	
	uint8_t cache_line_sz;
	uint8_t latency_timer;
	uint8_t header_type;
	uint8_t bist;

	uint32_t bar_0;

	uint32_t bar_1;
	
	uint32_t bar_2;
	
	uint32_t bar_3;
	
	uint32_t bar_4;
	
	uint32_t bar_5;

	uint32_t cardbus_cis_ptr;

	uint16_t subsys_vendor_id;
	uint16_t subsys_id;

	uint32_t expansion_rom_addr ;

	uint8_t capabilities_ptr;
	uint8_t reserved_0;
	uint16_t reserved_1;

	uint32_t reserved_2;

	uint8_t interrupt_line;
	uint8_t interrupt_pin;
	uint8_t min_grant;
	uint8_t max_latency;
} pci_header_type_0;

typedef struct _pci_header_type_1 {
	uint16_t vendor_id; 
	uint16_t device_id;

	uint16_t command; 
	uint16_t status;

	uint8_t revision_id; 
	uint8_t prog_if;
	uint8_t subclass;
	uint8_t class_code; 
	
	uint8_t cache_line_sz;
	uint8_t latency_timer;
	uint8_t header_type;
	uint8_t bist;

	uint32_t bar_0;

	uint32_t bar_1;

	uint8_t primary_bus_num;
	uint8_t secondary_bus_num;
	uint8_t subordinate_bus_num;
	uint8_t secondary_latency_timer;

	uint8_t io_base;
	uint8_t io_limit;
	uint16_t secondary_status;

	uint16_t memory_base;
	uint16_t memory_limit;

	uint16_t prefetchable_memory_base;
	uint16_t prefetchable_memory_limit;

	uint32_t prefetchable_base_upper_32;

	uint32_t prefetchable_limit_upper_32;

	uint16_t io_base_upper_16;
	uint16_t io_limit_upper_16;

	uint8_t capabilities_ptr;
	uint8_t reserved_0;
	uint16_t reserved_1;

	uint32_t expansion_rom_addr;

	uint8_t interrupt_line;
	uint8_t interrupt_pin;
	uint16_t bridge_control;

} pci_header_type_1;

typedef struct _pci_header_type_2 {
	uint16_t vendor_id; 
	uint16_t device_id;

	uint16_t command; 
	uint16_t status;

	uint8_t revision_id; 
	uint8_t prog_if;
	uint8_t subclass;
	uint8_t class_code; 
	
	uint8_t cache_line_sz;
	uint8_t latency_timer;
	uint8_t header_type;
	uint8_t bist;

	uint32_t cardbus_socket_base_addr;

	uint8_t capabilities_list_offset;
	uint8_t reserved_0;
	uint16_t secondary_status;

	uint8_t pci_bus_num;
	uint8_t card_bus_num;
	uint8_t subordinate_bus_num;
	uint8_t cardbus_latency_timer;

	uint32_t mem_base_0;

	uint32_t mem_limit_0;

	uint32_t mem_base_1;

	uint32_t mem_limit_1;

	uint32_t io_base_0;

	uint32_t io_limit_0;

	uint32_t io_base_1;

	uint32_t io_limit_1;

	uint8_t interrupt_line;
	uint8_t interrupt_pin;
	uint16_t bridge_control;

	uint16_t subsys_device_id;
	uint16_t subsys_vendor_id;

	uint32_t pc_card_legacy_mode_base;

} pci_header_type_2;



void		init_pci				();

void		brute_force_devs		();

uint16_t 	get_vendor_id			(uint8_t bus, uint8_t dev, uint8_t func);
uint16_t 	get_device_id			(uint8_t bus, uint8_t dev, uint8_t func);

uint16_t	get_status				(uint8_t bus, uint8_t dev, uint8_t func);
uint16_t	get_command				(uint8_t bus, uint8_t dev, uint8_t func);

uint8_t		get_class_code 			(uint8_t bus, uint8_t dev, uint8_t func);
uint8_t 	get_subclass_code		(uint8_t bus, uint8_t dev, uint8_t func);
uint8_t 	get_prog_if				(uint8_t bus, uint8_t dev, uint8_t func);
uint8_t 	get_revision_id			(uint8_t bus, uint8_t dev, uint8_t func);

uint8_t 	get_bist 				(uint8_t bus, uint8_t dev, uint8_t func);
uint8_t	 	get_header_type			(uint8_t bus, uint8_t dev, uint8_t func);
uint8_t 	get_latency_timer		(uint8_t bus, uint8_t dev, uint8_t func);
uint8_t 	get_cache_line_sz		(uint8_t bus, uint8_t dev, uint8_t func);

uint32_t 	get_bar_0				(uint8_t bus, uint8_t dev, uint8_t func);

uint32_t 	get_bar_1				(uint8_t bus, uint8_t dev, uint8_t func);

uint32_t 	get_bar_2				(uint8_t bus, uint8_t dev, uint8_t func);

uint32_t 	get_bar_3				(uint8_t bus, uint8_t dev, uint8_t func);

uint32_t 	get_bar_4				(uint8_t bus, uint8_t dev, uint8_t func);

uint32_t 	get_bar_5				(uint8_t bus, uint8_t dev, uint8_t func);

uint32_t 	get_cardbus_cis_ptr		(uint8_t bus, uint8_t dev, uint8_t func);

uint16_t 	get_subsys_id 			(uint8_t bus, uint8_t dev, uint8_t func);
uint16_t 	get_subsys_vendor_id 	(uint8_t bus, uint8_t dev, uint8_t func);

uint32_t 	get_expansion_rom_addr 	(uint8_t bus, uint8_t dev, uint8_t func);

uint8_t 	get_capabilities_ptr 	(uint8_t bus, uint8_t dev, uint8_t func);

uint8_t 	get_max_latency 		(uint8_t bus, uint8_t dev, uint8_t func);
uint8_t 	get_min_grant 			(uint8_t bus, uint8_t dev, uint8_t func);
uint8_t 	get_interrupt_pin 		(uint8_t bus, uint8_t dev, uint8_t func);
uint8_t 	get_interrupt_line 		(uint8_t bus, uint8_t dev, uint8_t func);

uint32_t	query_pci				(uint8_t bus, uint8_t dev, uint8_t func, uint8_t reg);


#endif
