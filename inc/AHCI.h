#ifndef _AHCI_
#define _AHCI_

#include "TextPrint.h"
#include "Typedefs.h"
#include "PageTable.h"
#include "PCI.h"

#define HBA_PORT_DEV_PRESENT 0x3
#define HBA_PORT_IPM_ACTIVE 0x1
#define SATA_SIG_ATAPI 0xEB140101
#define SATA_SIG_ATA 0x00000101
#define SATA_SIG_SEMB 0xC33C0101
#define SATA_SIG_PM 0x96690101

#define HBA_PxCMD_CR 0x8000
#define HBA_PxCMD_FRE 0x0010
#define HBA_PxCMD_ST 0x0001
#define HBA_PxCMD_FR 0x4000

#define ATA_DEV_BUSY 0x80
#define ATA_DEV_DRQ 0x08
#define ATA_CMD_READ_DMA_EX 0x25

#define HBA_PxIS_TFES (1 << 30)
#define LBA_MODE 1<<6

extern uint32_t ahci_base_addr; 

typedef enum _ahci_port_type {
	none = 0,
	sata = 1,
	semb = 2,
	pm = 3,
	satapi = 4,
} ahci_port_type;

typedef enum _fis_type {
	fis_type_reg_h2d = 0x27,
	fis_type_reg_d2h = 0x34,
	fis_type_dma_act = 0x39,
	fis_type_dma_setup = 0x41,
	fis_type_data = 0x46,
	fis_type_bist = 0x58,
	fis_type_pio_setup = 0x5F,
	fis_type_dev_bits = 0xA1
} fis_type;

typedef struct _hba_port {
	uint32_t command_list_base_addr_lower;
	uint32_t command_list_base_addr_upper;
	uint32_t fis_base_addr_lower;
	uint32_t fis_base_addr_upper;
	uint32_t interrupt_status;
	uint32_t interrupt_enable;
	uint32_t command_status;
	uint32_t reserved_0;
	uint32_t task_file_data;
	uint32_t signature;
	uint32_t sata_status;
	uint32_t sata_control;
	uint32_t sata_error;
	uint32_t sata_active;
	uint32_t command_issue;
	uint32_t sata_notification;
	uint32_t fis_based_switching_control;
	uint32_t reserved_1[11];
	uint32_t vendor_specific[4];
} hba_port;

typedef struct _hba_memory {
	uint32_t host_capabilities;
	uint32_t global_host_control;
	uint32_t interrupt_status;
	uint32_t ports_implemented;
	uint32_t version;
	uint32_t command_completion_coalescing_control;
	uint32_t command_completion_coalescing_ports;
	uint32_t enclosure_management_location;
	uint32_t enclosure_management_control;
	uint32_t host_capabilities_extended;
	uint32_t bios_handoff_control_status;
	uint8_t reserved_0[0x74];
	uint8_t vendor_specific[0x60];
	hba_port ports[1]; // atleast 2 ports, maybe up to 32
} hba_memory;

typedef struct _hba_command_header {
	uint8_t command_fis_length	: 5;
	uint8_t atapi 				: 1;
	uint8_t write 				: 1;
	uint8_t prefetchable 		: 1;

	uint8_t reset 				: 1;
	uint8_t bist 				: 1;
	uint8_t clear_busy 			: 1;
	uint8_t reserved_0 			: 1;
	uint8_t port_multiplier 	: 4;

	uint16_t phys_region_descriptor_table_length;
	uint32_t phys_region_descriptor_byte_count;
	uint32_t command_table_base_addr_lower;
	uint32_t command_table_base_addr_upper;
	uint32_t reserved_1[4];
} hba_command_header;

// AidanOS struct to store important info about hba_ports for easy access
typedef struct _hba_port_tracker {
	hba_port* hba_port_ptr; 
	ahci_port_type port_type;
	uint8_t* buffer; 
	uint8_t port_num;
} hba_port_tracker;

typedef struct _hba_prdt_entry {
    uint32_t data_base_addr_lower;
    uint32_t data_base_addr_upper;
    uint32_t reserved_0;

    uint32_t byte_count 				:22;
    uint32_t reserved_1 				:9;
    uint32_t interrupt_on_completion 	:1;
} hba_prdt_entry;

typedef struct _hba_command_table {
    uint8_t command_fis[64];
    uint8_t atapi_command[16];
    uint8_t reserved[48];
    hba_prdt_entry prdt_entry[];
} hba_command_table;

typedef struct _fis_reg_h2d {
	uint8_t fis_type;

	uint8_t port_multiplier		:4;
	uint8_t reserved_0			:3;
	uint8_t command_control 	:1;

	uint8_t command;
	uint8_t feature_low;

	uint8_t lba_0;
	uint8_t lba_1;
	uint8_t lba_2;
	uint8_t device_register;

	uint8_t lba_3;
	uint8_t lba_4;
	uint8_t lba_5;
	uint8_t feature_high;

	uint8_t count_low;
	uint8_t count_high;
	uint8_t iso_command_completion;
	uint8_t control;

	uint8_t reserved_1[4];
} fis_reg_h2d;

void init_ahci ();

void probe_ports ();

ahci_port_type get_port_type (hba_port* port);

void configure_port (hba_port* hba_port_ptr);

void port_stop_command (hba_port* hba_port_ptr);
void port_start_command (hba_port* hba_port_ptr);

uint8_t disk_read(uint64_t sector, uint32_t sector_count, void* buffer, hba_port* hba_port_ptr);

#endif