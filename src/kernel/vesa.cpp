#include "vesa.h"
VbeInfoBlock* vbeInfo;
vbe_mode_info_structure* VbeModeInfo;

void initVBEDevice(multiboot_info_t* mbt){
    print_serial("initVBEDevice");

    vbeInfo = (VbeInfoBlock*) mbt->vbe_control_info;
    printf_serial("Signature: %s, V0x%x\n", vbeInfo->VbeSignature, vbeInfo->VbeVersion);

    VbeModeInfo = (vbe_mode_info_structure*) mbt->vbe_mode_info;

    printf_serial("VESA video mode info: Width: %d Height: %d BPP: %d\n", VbeModeInfo->width, VbeModeInfo->height , VbeModeInfo->bpp);
    printf_serial("VideoMemory Location: 0x%x \n", VbeModeInfo->framebuffer );
}

void putPixel( int x, int y , uint32_t colour){
    printf_serial("putPixel x: %d, y: %d\n", x, y);
                    ///fb + mbt->framebuffer_pitch * y + 4 * x ,NOTE: this calculation is very important 
    *(uint32_t*) (  VbeModeInfo->framebuffer + VbeModeInfo->pitch * y + 4  * x ) = colour;
}
void drawLine(int x1, int y1, int x2, int y2, uint32_t colour ){
    print_serial("drawline\n");
    // Bresenham's line algorithm??
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    int D = 2 * deltaY - deltaX;
    int y = y1;

    for ( int x = x1; x < x2; x++){
      putPixel(x,y, colour);
      if( D > 0){
        y +=1;
        D = D - 2 * deltaX;
      }
      D = D + 2 * deltaY;
    }



}
void drawRect ( int x, int y, int width, int height, uint32_t colour ){
    print_serial("drawRect\n");
}


void blueDiagnalLineTest(multiboot_info_t* mbt){
    multiboot_uint32_t color;
    unsigned i;
    void *fb = (void *) (unsigned long) mbt->framebuffer_addr;

     switch (mbt->framebuffer_type)
        {
        case MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED:
          {
            unsigned best_distance, distance;
            struct multiboot_color *palette;
            
            palette = (struct multiboot_color *) mbt->framebuffer_palette_addr;

            color = 0;
            best_distance = 4*256*256;
            
            for (i = 0; i < mbt->framebuffer_palette_num_colors; i++)
              {
                distance = (0xff - palette[i].blue) * (0xff - palette[i].blue)
                  + palette[i].red * palette[i].red
                  + palette[i].green * palette[i].green;
                if (distance < best_distance)
                  {
                    color = i;
                    best_distance = distance;
                  }
              }
          }
          break;

        case MULTIBOOT_FRAMEBUFFER_TYPE_RGB:
          color = ((1 << mbt->framebuffer_blue_mask_size) - 1) 
            << mbt->framebuffer_blue_field_position;
          break;

        case MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT:
          color = '\\' | 0x0100;
          break;

        default:
          color = 0xffffffff;
          break;
        }
      for (i = 0; i < mbt->framebuffer_width
             && i < mbt->framebuffer_height; i++)
        {
          switch (mbt->framebuffer_bpp)
            {
            case 8:
              {
                multiboot_uint8_t *pixel = (multiboot_uint8_t*)fb + mbt->framebuffer_pitch * i + i;
                *pixel = color;
              }
              break;
            case 15:
            case 16:
              {
                multiboot_uint16_t *pixel
                  = (multiboot_uint16_t*)fb + mbt->framebuffer_pitch * i + 2 * i;
                *pixel = color;
              }
              break;
            case 24:
              {
                multiboot_uint32_t *pixel
                  = (multiboot_uint32_t*)fb + mbt->framebuffer_pitch * i + 3 * i;
                *pixel = (color & 0xffffff) | (*pixel & 0xff000000);
              }
              break;

            case 32:
              {
                multiboot_uint32_t *pixel
                  = (multiboot_uint32_t*)fb + mbt->framebuffer_pitch * i + 4 * i;
                *pixel = color;
              }
              break;
            }
        }
}

