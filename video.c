typedef enum COLOUR_TAG {
	BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, WHITE,
	GRAY, LIGHT_BLUE, LIGHT_GREEN, LIGHT_CYAN, 
	LIGHT_RED, LIGHT_MAGENTA, YELLOW, BRIGHT_WHITE
} COLOUR;

void clrscr()
{
    unsigned char *vidmem = (unsigned char *) 0xB8000;
    const long size = 80 * 25;
    long loop;
    
    // Clear visible video memory
    for (loop=0; loop<size; loop++) {
        *vidmem++ = 0;
        *vidmem++ = 0x0F;
    }
    
    // Set cursor position to 0,0
    out(0x3D4, 14);
    out(0x3D5, 0);
    out(0x3D4, 15);
    out(0x3D5, 0);
}
void print(const char *_message){
    unsigned short offset;
    // Read cursor position
    out(0x3D4, 14);
    offset = in(0x3D5) << 8;
    out(0x3D4, 15);
    offset |= in(0x3D5);

    unsigned char *vidmem = (unsigned char *) 0xB8000;
    vidmem += offset*2;
    int i = 0;
    
    while(_message[i]!=0){
        *vidmem++ = _message[i]; 
        *vidmem++ = 0x0F;
        i++;
    }
    offset += i ;
    out(0x3D5, (unsigned char)(offset));
    out(0x3D4, 14);
    out(0x3D5, (unsigned char)(offset >> 8));

}
void printf(const char *_message, COLOUR fg, COLOUR bg)
{
    unsigned short offset;
    unsigned long i;
    unsigned char *vidmem = (unsigned char *)0xB8000;
    char attr = (char)(bg<<4|fg);
    
    // Read cursor position
    out(0x3D4, 14);
    offset = in(0x3D5) << 8;
    out(0x3D4, 15);
    offset |= in(0x3D5);
    
    // Start at writing at cursor position
    vidmem += offset*2;
    
    // Continue until we reach null character
    i = 0;
    while (_message[i] != 0) {
        *vidmem++ = _message[i++];
        *vidmem++ = attr;
    }
    
    // Set new cursor position
    offset += i;
    out(0x3D5, (unsigned char)(offset));
    out(0x3D4, 14);
    out(0x3D5, (unsigned char)(offset >> 8));
}
