#ifndef _PCI_Class_Codes_ 
#define _PCI_Class_Codes_ 

#include "Typedefs.h"
#include "TextPrint.h"

#define PCI_CLASS_COUNT

char* 	lookup_class_code	(uint8_t class_code);
char* 	lookup_vendor_id	(uint16_t vendor_id);
char* 	lookup_device_id 	(uint16_t vendor_id, uint16_t device_id);
char* 	lookup_subclass 	(uint8_t class_code, uint8_t subclass);
char* 	lookup_prog_if 		(uint8_t class_code, uint8_t subclass, uint8_t prog_if);


#endif