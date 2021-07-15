pci_check:
	dd 0
	GLOBAL pci_check

DetectPCI:
    mov     ax, 0b101h				;; interrupt 1a function b101
    int     1ah						;; will tell us if there is a PCI
    mov		[pci_check], edx		;;EDX=20494350h
    ret