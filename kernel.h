#ifndef _KERNEL_
#define _KERNEL_

#include "inc/Typedefs.h"
#include "inc/TextPrint.h"
#include "inc/TextModeColorCodes.h"
#include "inc/IDT.h"
#include "inc/KeyboardHandler.h"
#include "inc/MemoryMap.h"
#include "inc/Bitmap.h"
#include "inc/PageAllocator.h"
#include "inc/PageTable.h"
#include "inc/Heap.h"
#include "inc/Mouse.h"
#include "inc/PCI.h"
#include "inc/PIT.h"
#include "inc/Serial.h"
#include "inc/PCIClassCodes.h"
#include "inc/AHCI.h"
#include "inc/Process.h"
#include "inc/UHCI.h"
#include "inc/USB.h"


void _start();

#endif