

unsigned long crc_lookup[4]={0,0xEDB88320,0x76DC4190,0x9B64C2B0};

unsigned int crc32(unsigned long crc, unsigned char *addr, int num)
{
#asm
	push bp
	mov bp,sp
	
#if !__FIRST_ARG_IN_AX__
	#define ARG_ADDR bp+8
	#define ARG_NUM bp+10	
	mov dx,[bp+6]
	mov	bx,[bp+4]
#else
	#define ARG_ADDR bp+4
	#define ARG_NUM bp+6	
	mov dx,bx	; hi
	mov bx,ax   ; lo
#endif	
	mov si,[ARG_ADDR]     ; si:=addr
	mov di,#_crc_lookup
	mov cx,[ARG_NUM] ; num

	xor ax,ax
	not ax
	xor bx,ax
	xor dx,ax

__crc_process_byte:	
	lodsb
	xor bl,al
	mov ah,#0x4
__crc_process2bits:
	xor bp,bp
	shr dx,1
	rcr bx,1
	rcl bp,1
	shr dx,1
	rcr bx,1
	rcl bp,1
	add bp,bp
	add bp,bp
	xor bx,[di+bp]
	xor dx,[di+bp+2]
	dec ah
	jnz __crc_process2bits
	dec cx
	jnz __crc_process_byte
	xor ax,ax
	not ax
	xor dx,ax
	xor ax,bx
	pop	bp
	
#endasm
}
