.586
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib "..DebugStaticLib2.lib"


.stack 65536
ExitProcess PROTO:DWORD 

 outnumline PROTO : DWORD
 outstrline PROTO : DWORD
 power PROTO : DWORD
 strcomp PROTO : DWORD
 pausese PROTO :
 input PROTO : DWORD
 _ConvertToChar PROTO : DWORD

.const
	printline byte 13, 10, 0	
	_5 dword 5
	_10_name_ byte ' 10 ', 0
	_0x20_name_ byte ' 0x20 ', 0
	_2_name_ byte ' 2 ', 0

.data
	temp sdword ?
	buffer byte 256 dup(0)
	_10_integer_ dword 10
	_10_string_ byte '10', 0
	_10_hexinteger_ byte '10', 0
	_0x20_integer_ dword 020h
	_0x20_string_ byte '020h', 0
	_0x20_hexinteger_ byte '020h', 0
	_2_integer_ dword 2
	_2_string_ byte '2', 0
	_2_hexinteger_ byte '2', 0


.code
main PROC
	push _5
	push _0x20_integer_
	call _ConvertToChar
	mov offset _0x20_string_, eax
	mov offset _0x20_hexinteger_, eax

	push _10_integer_
	push _2_integer_
	pop eax
	pop ebx
	imul ebx
	push eax
	pop _0x20_integer_
	push _0x20_integer_
	call _ConvertToChar
	mov offset _0x20_string_, eax
	mov offset _0x20_hexinteger_, eax


push offset _0x20_string_
call outstrline
push 0
call pausese
push 0
call ExitProcess
main ENDP
end main