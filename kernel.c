void kmain( void* mbd, unsigned int magic )
{
   if ( magic != 0x2BADB002 )
   {
      /* Something went not according to specs. Print an error */
      /* message and halt, but do *not* rely on the multiboot */
      /* data structure. */
   }
 
   /* You could either use multiboot.h */
   /* (http://www.gnu.org/software/grub/manual/multiboot/multiboot.html#multiboot_002eh) */
   /* or do your offsets yourself. The following is merely an example. */ 
   //char * boot_loader_name =(char*) ((long*)mbd)[16];
 
   /* Print a letter to screen to see everything is working: */
    unsigned char *videoram = (unsigned char *) 0xb8000;
    videoram[0] = 'E'; /* character 'A' */
    videoram[1] = 0x07; /* light grey (7) on black (0). */
    videoram[2] = 'N'; /* character 'A' */
    videoram[3] = 0x07; /* light grey (7) on black (0). */
    videoram[4] = 'D'; /* character 'A' */
    videoram[5] = 0x07; /* light grey (7) on black (0). */
    videoram[6] = 'I'; /* character 'A' */
    videoram[7] = 0x07; /* light grey (7) on black (0). */
    videoram[8] = 'K'; /* character 'A' */
    videoram[9] = 0x07; /* light grey (7) on black (0). */
    videoram[10] = 'A'; /* character 'A' */
    videoram[11] = 0x07; /* light grey (7) on black (0). */
}
