#ifdef __AS386_16__
#asm
  loc	1		! Make sure the pointer is in the correct segment
auto_func:		! Label for bcc -M to work.
  .word	___mkargv	! Pointer to the autorun function
  .text			! So the function after is also in the correct seg.
#endasm
#endif

extern int __psp;
extern int __envseg;

void set_program_name(__argv)
char ** __argv;
{
	unsigned char *ptr;
	int src,len;
	
	__set_es(__envseg);
	src=0;
	while(__peek_es(src++)!=0)
	{
		while(__peek_es(src++)!=0);
	}
	src+=2; // step over 0x0001
	len=0;
	while (__peek_es(src+len++)!=0);
	ptr=sbrk(len);
	__argv[0]=ptr;
	while(len--)
	{
		*ptr++=__peek_es(src++);
	}
}

__mkargv(__argc, __argv)
int __argc;
char ** __argv;
{
   int length, i, argc=1, s=0;
   unsigned char *ptr, *p;
   
   set_program_name(__argv);
   
   __set_es(__psp);			/* Pointer to the args */
   length = __peek_es(0x80); 		/* Length of cmd line */
   if( length > 0 )
   {
      ptr = (char*) sbrk(length+1);	/* Allocate some space */

      for(i=0; i<length; i++)		/* Copy it in. */
      {
	ptr[i] = __peek_es(0x81+i);
	if( ptr[i] >  ' ' && s == 0 ) { argc++; s=1; }
	if( ptr[i] <= ' ' && s == 1 ) s=0;
      }
      ptr[length]=0;

      p= __argv[0];
      __argv = (char**) sbrk((argc+1)*sizeof(char*));
      __argv[0] = p;		/* TODO: The real command can be found */
      __argc=argc;

      /*
       * TODO: This needs to understand quoting and wildcards 
       */

      argc=1; s=0;
      for(i=0; i<length; i++)
      {
	if( ptr[i] >  ' ' && s == 0 ) { __argv[argc++] = ptr+i; s=1; }
	if( ptr[i] <= ' ' && s == 1 ) { ptr[i] = '\0'; s=0; }
      }
      __argv[argc] = 0;
   }
}
