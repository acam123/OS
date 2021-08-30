 #include "PCIClassCodes.h"

char* lookup_class_code(uint8_t class_code) {
    switch (class_code) {
        case 0x00:
            return "Unclassified";
        case 0x01:
            return "Mass Storage Controller";
        case 0x02:
            return "Network Controller";
        case 0x03:
            return "Display Controller";
        case 0x04:
            return "Multimedia Controller";
        case 0x05:
            return "Memory Controller";
        case 0x06:
            return "Bridge Device";
        case 0x07:
            return "Simple Communication Controller";
        case 0x08:
            return "Base System Peripheral";
        case 0x09:
            return "Input Device Controller";
        case 0x0a:
            return "Docking Station"; 
        case 0x0b:
            return "Processor";
        case 0x0c:
            return "Serial Bus Controller";
        case 0x0d: 
            return "Wireless Controller";
        case 0x0e: 
            return "Intelligent Controller";
        case 0x0f:
            return "Satellite Communication Controller";
        case 0x10:
            return "Encryption Controller";
        case 0x11:
            return "Signal Processing Controller";
        case 0x12:
            return "Processing Accelerator";
        case 0x13: 
            return "Non-Essential Instrumentation";
        case 0x40:
            return "Co-Processor";
        case 0xff:
            return "Unassigned Class";
        default:
            return hex_to_str(class_code);
    }

}



// Unclassified 
char* lookup_subclass_00 (uint8_t subclass) {
    switch (subclass) {
        case 0x00:
            return "Non-VGA-Compatible";
        case 0x01:
            return "VGA-Compatible";
        default:
            return hex_to_str(subclass);
    }
}


// Mass Storage Controller
char* lookup_subclass_01 (uint8_t subclass) { 
    switch (subclass) {
        case 0x00:
            return "SCSI Bus Controller";
        case 0x01:
            return "IDE Controller";
        case 0x02:
            return "Floppy Disk Controller";
        case 0x03: 
            return "IPI Bus Controller";
        case 0x04:
            return "RAID Controller";
        case 0x05:
            return "ATA Controller";
        case 0x06:
            return "Serial ATA";
        case 0x07:
            return "Serial Attached SCSI";
        case 0x08:
            return "Non-Volatile Memory Controller";
        case 0x80:
            return "Other";
        default:
            return hex_to_str(subclass);
    }
}

// Network Controller
char* lookup_subclass_02 (uint8_t subclass) {
    switch (subclass) {
        case 0x00:
            return "Ethernet Controller";
        case 0x01:
            return "Token Ring Controller";
        case 0x02: 
            return "FDDI Controller";
        case 0x03:
            return "ATM Controller";
        case 0x04: 
            return "ISDN Controller";
        case 0x05:
            return "WorldFip Controller";
        case 0x06:
            return "PICMG 2.14 Multi Computing Controller";
        case 0x07:
            return "Infiniband Controller";
        case 0x08:
            return "Fabric Controller";
        case 0x80: 
            return "Other";
        default:
            return hex_to_str(subclass);
    }
}


// Display Controller
char* lookup_subclass_03 (uint8_t subclass) {
    switch (subclass) {
        case 0x00:
            return "VGA Compatible Controller";
        case 0x01: 
            return "XGA Controller";
        case 0x02:
            return "3D Controller (Not VGA-Compatible)";
        case 0x80:
            return "Other";
        default:
            return hex_to_str(subclass);
    }
}


// Multimedia Controller
char* lookup_subclass_04 (uint8_t subclass) {
    switch (subclass) {
        case 0x00:
            return "Multimedia Video Controller";
        case 0x01: 
            return "Multimedia Audio Controller";
        case 0x02:
            return "Computer Telephony Device";
        case 0x03:
            return "Audio Device";
        case 0x80: 
            return "Other";
        default:
            return hex_to_str(subclass);
    }
}


// Memory Controller
char* lookup_subclass_05 (uint8_t subclass) {
    switch (subclass) {
        case 0x00:
            return "RAM Controller";
        case 0x01: 
            return "Flash Controller";
        case 0x80: 
            return "Other";
        default:
            return hex_to_str(subclass);
    }
}


// Bridge Controller
char* lookup_subclass_06 (uint8_t subclass) {
    switch (subclass) {
        case 0x00: 
            return "Host Bridge";
        case 0x01: 
            return "ISA Bridge";
        case 0x02: 
            return "EISA Bridge";
        case 0x03:
            return "MCA Bridge";
        case 0x04:
            return "PCI-to-PCI Bridge";
        case 0x05:
            return "PCMCIA Bridge";
        case 0x06:
            return "NuBus Bridge";
        case 0x07:
            return "CardBus Bridge";
        case 0x08:
            return "RACEway Bridge";
        case 0x09:
            return "PCI-to-PCI Bridge";
        case 0x0a:
            return "InfiniBand-to-PCI Host Bridge";
        case 0x80:
            return "Other";
        default:
            return hex_to_str(subclass);
    }
}


// Simple Communication Controller
char* lookup_subclass_07 (uint8_t subclass) {
    switch (subclass) {
        case 0x00:
            return "Serial Controller";
        case 0x01:
            return "Parallel Controller";
        case 0x02:
            return "Multiport Serial Controller";
        case 0x03:
            return "Modem";
        case 0x04:
            return "IEEE 488.1/2 (GPIB) Controller";
        case 0X05:
            return "Smart Card Controller";
        case 0x80:
            return "Other";
        default:
            return hex_to_str(subclass);
    }
}

// Base System Peripheral
char* lookup_subclass_08 (uint8_t subclass) {
    switch (subclass) {
        case 0x00:
            return "PIC";
        case 0x01:
            return "DMA Controller";
        case 0x02:
            return "Timer";
        case 0x03:
            return "RTC Controller";
        case 0x04:
            return "PCI Hot-Plug Controller";
        case 0x05:
            return "SD Host controller";
        case 0x06:
            return "IOMMU";
        case 0x80:
            return "Other";
        default:
            return hex_to_str(subclass);
    }
}

// Input Device Controller
char* lookup_subclass_09 (uint8_t subclass) {
    switch (subclass) {
        case 0x00:
            return "Keyboard Controller";
        case 0x01:
            return "Digitizer Pen";
        case 0x02:
            return "Mouse Controller";
        case 0x03:
            return "Scanner Controller";
        case 0x04:
            return "Gameport Controller";
        case 0x80:
            return "Other";
        default:
            return hex_to_str(subclass);
    }
}

// Docking Station
char* lookup_subclass_0a (uint8_t subclass) { 
    switch (subclass) {
        case 0x00: 
            return "Generic";
        case 0x80: 
            return "Other";
        default:
            return hex_to_str(subclass);
    }
}

// Processor
char* lookup_subclass_0b (uint8_t subclass) { 
    switch (subclass) { 
        case 0x00:
           return "386";
        case 0x01:
            return "486";
        case 0x02:
            return "Pentium";
        case 0x03:
            return "Pentium Pro";
        case 0x10:
            return "Alpha";
        case 0x20: 
            return "PowerPC";
        case 0x30:
            return "MIPS";
        case 0x40:
            return "Co-Processor";
        case 0x80: 
            return "Other";
        default:
            return hex_to_str(subclass);
    }
}

// Serial Bus Controller
char* lookup_subclass_0c (uint8_t subclass) {  
    switch (subclass) {
        case 0x00:
            return "FireWire (IEEE 1394) Controller";
        case 0x01:
            return "ACCESS Bus";
        case 0x02:
            return "SSA";
        case 0x03:
            return "USB Controller";
        case 0x04:
            return "Fibre Channel";
        case 0x05:
            return "SMBus";
        case 0x06:
            return "Infiniband";
        case 0x07:
            return "IPMI Interface";
        case 0x08:
            return "SERCOS Interface (IEC 61491)";
        case 0x09:
            return "CANbus";
        case 0x80:
            return "Other";
        default:
            return hex_to_str(subclass);
    }
}

// Wireless Controller
char* lookup_subclass_0d (uint8_t subclass) {  
    switch (subclass) {
        case 0x00:
            return "iRDA Compatible Controller";
        case 0x01:
            return "Consumer IR Controller";
        case 0x10:
            return "RF Controller";
        case 0x11:
            return "Bluetooth Controller";
        case 0x12: 
            return "Broadband Controller";
        case 0x20:
            return "Ethernet Controller (802.1a)";
        case 0x21: 
            return "Ethernet Controller (802.1b)";
        case 0x80:
            return "Other";
        default:
            return hex_to_str(subclass);
    }
}

// Intelligent Controller
char* lookup_subclass_0e (uint8_t subclass) {  
    switch (subclass) {
        case 0x00:
            return "I20";
        default:
            return hex_to_str(subclass);
    }
}

// Satellite Communication Controller
char* lookup_subclass_0f (uint8_t subclass) {  
    switch (subclass) {
        case 0x01:
            return "Satellite TV Controller";
        case 0x02:
            return "Satellite Audio Controller";
        case 0x03:
            return "Satellite Voice Controller";
        case 0x04: 
            return "Satellite Data Controller";
        default:
            return hex_to_str(subclass);
    }
}

// Encryption Controller
char* lookup_subclass_10 (uint8_t subclass) {  
    switch (subclass) {
        case 0x00:
            return "Network and Computing Encrpytion/Decryption";
        case 0x10: 
            return "Entertainment Encryption/Decryption";
        case 0x80:
            return "Other";
        default:
            return hex_to_str(subclass);
    }
}

// Signal Processing Controller
char* lookup_subclass_11 (uint8_t subclass) {  
    switch (subclass) {
        case 0x00:
            return "DPIO Modules";
        case 0x01: 
            return "Performance Counters";
        case 0x10:
            return "Communication Synchronizer";
        case 0x20:
            return "Signal Processing Management";
        case 0x80:
            return "Other";
        default:
            return hex_to_str(subclass);
    }
}

char* lookup_subclass (uint8_t class_code, uint8_t subclass) {
    switch (class_code) {
        case 0x00:
            return lookup_subclass_00(subclass);
        case 0x01:
            return lookup_subclass_01(subclass);
        case 0x02:
            return lookup_subclass_02(subclass);
        case 0x03:
            return lookup_subclass_03(subclass);
        case 0x04:
            return lookup_subclass_04(subclass);
        case 0x05:
            return lookup_subclass_05(subclass);
        case 0x06:
            return lookup_subclass_06(subclass);
        case 0x07:
            return lookup_subclass_07(subclass);
        case 0x08:
            return lookup_subclass_08(subclass);
        case 0x09:
            return lookup_subclass_09(subclass);
        case 0x0a:
            return lookup_subclass_0a(subclass);
        case 0x0b:
            return lookup_subclass_0b(subclass);
        case 0x0c:
            return lookup_subclass_0c(subclass);
        case 0x0d:
            return lookup_subclass_0d(subclass);
        case 0x0e:
            return lookup_subclass_0e(subclass);
        case 0x0f:
            return lookup_subclass_0f(subclass);
        case 0x10:
            return lookup_subclass_10(subclass);
        case 0x11:
            return lookup_subclass_11(subclass);
        default:
            return hex_to_str(subclass);
    }
}

char* lookup_vendor_id(uint16_t vendor_id) {
    switch (vendor_id) {
        case 0x8086:
            return "Intel Corp.";
        case 0x1234:
            return "Technical Corp."; 
        default:
            return hex_to_str(vendor_id);
    }
}

char* lookup_device_id_8086 (uint16_t device_id) {
    switch (device_id) {
        case 0x10d3:
            return "82574L Gigabit Network Connection";
        case 0x1237:
            return "440FX - 82441FX PMC [Natoma]";
        case 0x2918:
            return "82801IB (ICH9) LPC Interface Controller";
        case 0x2934:
            return "82801I (ICH9 Family) USB UHCI Controller #1";
        case 0x2935:
            return "82801I (ICH9 Family) USB UHCI Controller #2";
        case 0x2936:
            return "82801I (ICH9 Family) USB UHCI Controller #3";
        case 0x293a:
            return "82801I (ICH9 Family) USB2 EHCI Controller #1";
        case 0x29c0:
            return "82G33/G31/P35/P31 Express DRAM Controller";
        case 0x2922:
            return "82801IR/IO/IH (ICH9R/DO/DH) 6 port SATA Controller [AHCI mode]";
        case 0x2930:
            return "82801I (ICH9 Family) SMBus Controller";
        case 0x7000:
            return "82371SB PIIX3 ISA [Natoma/Triton II]";
        case 0x7010:
            return "82371SB PIIX3 IDE [Natoma/Triton II]";
        case 0x7020:
            return "82371SB PIIX3 USB [Natoma/Triton II]";
        case 0x7110:    
            return "82371AB/EB/MB PIIX4 ISA"; 
        case 0x7111:    
            return "82371AB/EB/MB PIIX4 IDE"; 
        case 0x7112:    
            return "82371AB/EB/MB PIIX4 USB";
        case 0x7113:
            return "82371AB/EB/MB PIIX4 ACPI";
        case 0x100e:
            return "82540EM Gigabit Ethernet Controller";
        case 0x24cd:
            return "82801DB/DBM (ICH4/ICH4-M) USB2 EHCI Controller";
        default:
            return hex_to_str(device_id);
    }
}

char* lookup_device_id_1234 (uint16_t device_id) {
    switch (device_id) {
        case 0x1111:
            return "QEMU Virtual Video Controller";
        default:
            return hex_to_str(device_id);
    }
}

char* lookup_device_id (uint16_t vendor_id, uint16_t device_id) { 
    switch (vendor_id) {
        case 0x8086:
            return lookup_device_id_8086(device_id);
        case 0x1234:
            return lookup_device_id_1234(device_id);
        default:
            return hex_to_str(device_id); 
    }
}

char* lookup_prog_if_01_01 (uint8_t prog_if) {
    switch (prog_if) {
        case 0x00:
            return "ISA Compatibility"; // mode-only controller
        case 0x05: 
            return "PCI"; // native mode-only controller
        case 0x0a:
            return "ISA Compatibility"; // mode controller, supports both channels switched to PCI native mode
        case 0x0f:
            return "PCI"; // native mode controller, supports both channels switched to ISA compatibility mode
        case 0x80:
            return "ISA Compatibility"; // mode-only controller, supports bus mastering
        case 0x85:
            return "PCI"; // native mode-only controller, supports bus mastering
        case 0x8a:
            return "ISA Compatibility"; // mode controller, supports both channels switched to PCI native mode, supports bus mastering
        case 0x8f:
            return "PCI"; // native mode controller, supports both channels switched to ISA compatibility mode, supports bus mastering
        default:
            return hex_to_str(prog_if);
    }
}

char* lookup_prog_if_01_05 (uint8_t prog_if) {
    switch (prog_if) {
        case 0x20:
            return "Single DMA";
        case 0x30:
            return "Chained DMA";
        default:
            return hex_to_str(prog_if);
    }
}

char* lookup_prog_if_01_06 (uint8_t prog_if) {
     switch (prog_if) {
        case 0x00:
            return "Vendor Specific Interface";
        case 0x01:
            return "AHCI 1.0";
        case 0x02:
            return "Serial Storage Bus";
        default:
            return hex_to_str(prog_if);
    }
}

char* lookup_prog_if_01_07 (uint8_t prog_if) {
    switch (prog_if) {
        case 0x00:
            return "SAS";
        case 0x01:
            return "Serial Storage Bus";
        default:
            return hex_to_str(prog_if);
    }
}

char* lookup_prog_if_01_08 (uint8_t prog_if) {
    switch (prog_if) {
        case 0x01: 
            return "NVMHCI";
        case 0x02: 
            return "NVM Express";
        default:
            return hex_to_str(prog_if);
    }
}

char* lookup_prog_if_01 (uint8_t subclass, uint8_t prog_if) {
    switch (subclass) {
        case 0x01:
            return lookup_prog_if_01_01(prog_if);
        case 0x05:
            return lookup_prog_if_01_05(prog_if);
        case 0x06:
            return lookup_prog_if_01_06(prog_if);
        case 0x07:
            return lookup_prog_if_01_07(prog_if);
        case 0x08:
            return lookup_prog_if_01_08(prog_if);
        default:
            return hex_to_str(prog_if);
    }
}

char* lookup_prog_if_03_00 (uint8_t prog_if) {
    switch (prog_if) {
        case 0x00:
            return "VGA Controller";
        case 0x01:
            return "8514-Compatible Controller";
         default:
            return hex_to_str(prog_if);
    }
}

char* lookup_prog_if_03 (uint8_t subclass, uint8_t prog_if) {
    switch (subclass) {
        case 0x00:
            return lookup_prog_if_03_00(prog_if);
        default:
            return hex_to_str(prog_if);
    }
}

char* lookup_prog_if_06_04 (uint8_t prog_if) {
    switch (prog_if) {
        case 0x00: 
            return "Normal Decode";
        case 0x01:
            return "Subtractive Decode";
        default:
            return hex_to_str(prog_if);
    }
}

char* lookup_prog_if_06_08 (uint8_t prog_if) {
    switch (prog_if) {
        case 0x00:
            return "Transparent Mode";
        case 0x01: 
            return "Endpoint Mode";
        default:
            return hex_to_str(prog_if);
    }
}

char* lookup_prog_if_06_09 (uint8_t prog_if) {
    switch (prog_if) {
        case 0x40:
            return "Semi-Transparent, Primary";   // bus towards host CPU
        case 0x80:
            return "Semi-Transparent, Secondary"; // bus towards host CPU
         default:
            return hex_to_str(prog_if);
    }
}

char* lookup_prog_if_06 (uint8_t subclass, uint8_t prog_if) {
    switch (subclass) {
        case 0x04:
            return lookup_prog_if_06_04(prog_if);
        case 0x08:
            return lookup_prog_if_06_08(prog_if);
        case 0x09:
            return lookup_prog_if_06_09(prog_if);
        default:
            return hex_to_str(prog_if);
    }   
}

char* lookup_prog_if_07_00 (uint8_t prog_if) {
    switch (prog_if) {
        case 0x00:
            return "8250-Compatible (Generic XT)";
        case 0x01:
            return "16450-Compatible";
        case 0x02:
            return "16550-Compatible";
        case 0x03: 
            return "16650-Compatible";
        case 0x04:
            return "16750-Compatible";
        case 0x05:
            return "16850-Compatible";
        case 0x06:
            return "16950-Compatible";
        default:
            return hex_to_str(prog_if);
    }
}

char* lookup_prog_if_07_01 (uint8_t prog_if) {
    switch (prog_if) {
        case 0x00:
            return "Standard Parallel Port";
        case 0x01:
            return "Bi-Directional Parallel Port";
        case 0x02: 
            return "ECP 1.X Compliant Parallel Port";
        case 0x03:
            return "IEEE 1284 Controller";
        case 0xfe:
            return "IEEE 1284 Target Device";
        default:
            return hex_to_str(prog_if);
    }
}

char* lookup_prog_if_07_03 (uint8_t prog_if) {
    switch (prog_if) {
        case 0x00:
            return "Generic Modem";
        case 0x01: 
            return "Hayes 16450-Compatible Interface";
        case 0x02:
            return "Hayes 16550-Compatible Interface";
        case 0x03:
            return "Hayes 16650-Compatible Interface";
        case 0x04:
            return "Hayes 16750-Compatible Interface";
        default:
            return hex_to_str(prog_if);
    }
}

char* lookup_prog_if_07 (uint8_t subclass, uint8_t prog_if) {
     switch (subclass) {
        case 0x00:
            return lookup_prog_if_07_00(prog_if);
        case 0x01:
            return lookup_prog_if_07_01(prog_if);
        case 0x03:
            return lookup_prog_if_07_03(prog_if);
        default:
            return hex_to_str(prog_if);
     }

}

char* lookup_prog_if_08_00 (uint8_t prog_if) {
    switch (prog_if) {
        case 0x00:
            return "Generic 8259-Compatible";
        case 0x01:
            return "ISA-Compatible";
        case 0x02:
            return "EISA-Compatible";
        case 0x10:
            return "I/O APIC Interrupt Controller";
        case 0x20:
            return "I/O(x) APIC Interrupt Controller";
        default:
            return hex_to_str(prog_if);
    }
}

char* lookup_prog_if_08_01 (uint8_t prog_if) {
    switch (prog_if) {
        case 0x00:
            return "Generic 8237-Compatible";
        case 0x01:
            return "ISA-Compatible";
        case 0x02:
            return "EISA-Compatible";
        default:
            return hex_to_str(prog_if);
    }
}

char* lookup_prog_if_08_02 (uint8_t prog_if) {
    switch (prog_if) {
        case 0x00:
            return "Generic 8254-Compatible";
        case 0x01:
            return "ISA-Compatible";
        case 0x02:
            return "EISA-Compatible";
        case 0x03:
            return "HPET";
        default:
            return hex_to_str(prog_if);
    }
}

char* lookup_prog_if_08_03 (uint8_t prog_if) {
    switch (prog_if) {
        case 0x00:
            return "Generic RTC";
        case 0x01:
            return "ISA-Compatible";
        default:
            return hex_to_str(prog_if);
    }
}

char* lookup_prog_if_08 (uint8_t subclass, uint8_t prog_if) {
    switch (subclass) {
        case 0x00:
            return lookup_prog_if_08_00(prog_if);
        case 0x01:
            return lookup_prog_if_08_01(prog_if);
        case 0x02:
            return lookup_prog_if_08_02(prog_if);
        case 0x03:
            return lookup_prog_if_08_03(prog_if);
        default:
            return hex_to_str(prog_if);
    }
}

char* lookup_prog_if_09_04 (uint8_t prog_if) {
    switch (prog_if) {
        case 0x00: 
            return "Generic";
        case 0x10: 
            return "Extended";
        default:
            return hex_to_str(prog_if);
    }
}

char* lookup_prog_if_09 (uint8_t subclass, uint8_t prog_if) {
    switch (subclass) {
        case 0x04:
            return lookup_prog_if_09_04(prog_if);
        default:
            return hex_to_str(prog_if);
    }
}

char* lookup_prog_if_0c_00 (uint8_t prog_if) {
    switch (prog_if) {
        case 0x00: 
            return "Generic";
        case 0x10:
            return "OHCI";
        default:
            return hex_to_str(prog_if);
    }
}

char* lookup_prog_if_0c_03 (uint8_t prog_if) {
    switch (prog_if) {
        case 0x00:
            return "UHCI Controller";
        case 0x10: 
            return "OHCI Controller";
        case 0x20:
            return "EHCI (USB2) Controller";
        case 0x30:
            return "XHCI (USB3) Controller";
        case 0x80:
            return "Unspecified";
        case 0xFE:
            return "USB Device (Not a host controller)";
        default:
            return hex_to_str(prog_if);
    }
}

char* lookup_prog_if_0c_07 (uint8_t prog_if) {
    switch (prog_if) {
        case 0x00:
            return "SMIC";
        case 0x01:
            return "Keyboard Controller Style";
        case 0x02:
            return "Block Transfer";
        default:
            return hex_to_str(prog_if);
    }
}

char* lookup_prog_if_0c (uint8_t subclass, uint8_t prog_if) {
    switch (subclass) {
        case 0x00:
            return lookup_prog_if_0c_00(prog_if);
        case 0x03:
            return lookup_prog_if_0c_03(prog_if);
        case 0x07:
            return lookup_prog_if_0c_07(prog_if);
        default:
            return hex_to_str(prog_if);
    }
}

char* lookup_prog_if (uint8_t class_code, uint8_t subclass, uint8_t prog_if) {
    switch (class_code) { 
        case 0x01:
            return lookup_prog_if_01(subclass, prog_if);
        case 0x03:
            return lookup_prog_if_03(subclass, prog_if);
        case 0x06:
            return lookup_prog_if_06(subclass, prog_if);
        case 0x07:
            return lookup_prog_if_07(subclass, prog_if);
        case 0x08:
            return lookup_prog_if_08(subclass, prog_if);
        case 0x09:
            return lookup_prog_if_09(subclass, prog_if);
        case 0x0c:
            return lookup_prog_if_0c(subclass, prog_if);
        default:
            return hex_to_str(prog_if);
    }
}
