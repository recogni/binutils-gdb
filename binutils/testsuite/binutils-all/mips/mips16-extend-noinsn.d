#PROG: objcopy
#objdump: -d --prefix-addresses --show-raw-insn
#name: MIPS16 unsupported EXTEND and undefined opcode disassembly
#as: -32

# Verify raw hexadecimal EXTEND and inexistent opcode disassembly.

.*: +file format .*mips.*

Disassembly of section \.text:
[0-9a-f]+ <[^>]*> 6500      	nop
[0-9a-f]+ <[^>]*> f123      	extend	0x123
[0-9a-f]+ <[^>]*> f456      	extend	0x456
[0-9a-f]+ <[^>]*> f765      	extend	0x765
[0-9a-f]+ <[^>]*> 1800 0000 	jal	00000000 <foo>
[0-9a-f]+ <[^>]*> 6500      	nop
[0-9a-f]+ <[^>]*> f432      	extend	0x432
[0-9a-f]+ <[^>]*> 1c00 0000 	jalx	00000000 <foo>
[0-9a-f]+ <[^>]*> 6500      	nop
[0-9a-f]+ <[^>]*> f123 6621 	0xf123 0x6621
[0-9a-f]+ <[^>]*> f456 e935 	0xf456 0xe935
[0-9a-f]+ <[^>]*> f765 ea60 	0xf765 0xea60
[0-9a-f]+ <[^>]*> f432 ece0 	0xf432 0xece0
[0-9a-f]+ <[^>]*> f5aa e971 	0xf5aa 0xe971
[0-9a-f]+ <[^>]*> f655 ebf1 	0xf655 0xebf1
[0-9a-f]+ <[^>]*> 6621      	0x6621
[0-9a-f]+ <[^>]*> e935      	0xe935
[0-9a-f]+ <[^>]*> ea60      	0xea60
[0-9a-f]+ <[^>]*> ece0      	0xece0
[0-9a-f]+ <[^>]*> e971      	0xe971
[0-9a-f]+ <[^>]*> ebf1      	0xebf1
[0-9a-f]+ <[^>]*> 6500      	nop
	\.\.\.
