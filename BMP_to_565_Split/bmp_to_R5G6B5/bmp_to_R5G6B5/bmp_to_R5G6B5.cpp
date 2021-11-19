//=============================================================================
//
//  CRYSTALFONTZ BMP to R5G6B5 data structure conversion code
//
//  Visual Studio 2017
//
//  ref: https://www.crystalfontz.com/
//
//  2018 - 11 - 03 Brent A. Crosby
//
//=============================================================================
//This is free and unencumbered software released into the public domain.
//
//Anyone is free to copy, modify, publish, use, compile, sell, or
//distribute this software, either in source code form or as a compiled
//binary, for any purpose, commercial or non-commercial, and by any
//means.
//
//In jurisdictions that recognize copyright laws, the author or authors
//of this software dedicate any and all copyright interest in the
//software to the public domain. We make this dedication for the benefit
//of the public at large and to the detriment of our heirs and
//successors. We intend this dedication to be an overt act of
//relinquishment in perpetuity of all present and future rights to this
//software under copyright law.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
//OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
//ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//OTHER DEALINGS IN THE SOFTWARE.
//
//For more information, please refer to <http://unlicense.org/>
//=============================================================================
//
// To make this static linked (no DLLs required)
//
//ref: https://social.msdn.microsoft.com/Forums/vstudio/en-US/845617aa-ab76-4fe1-a658-d020828598b8/static-linking-in-vs2010-runtime?forum=vcgeneral
//
//To set static linking:
//Configuration Properties, C/C++, Code Generation, Runtime Library -> MultiThreaded (/MT)
//In addition, if using MFC:
//Configuration Properties, General, Use of MFC -> Use MFC in a Static Library
//In addition, if using ATL:
//Configuration Properties, Genera, Use of ATL -> Static Link to ATL
//
//=============================================================================
#include "stdafx.h"
#include "windows.h"
#include "stdint.h"

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

//===========================================================================
//
//  ref http://stackoverflow.com/questions/14279242/read-bitmap-file-into-structure
//
//  These are the defines that are buried somewhere in windows.h
//
//typedef struct tagBITMAPFILEHEADER
//  {
//  WORD  bfType;       //specifies the file type
//  DWORD bfSize;       //specifies the size in bytes of the bitmap file
//  WORD  bfReserved1;  //reserved; must be 0
//  WORD  bfReserved2;  //reserved; must be 0
//  DWORD bOffBits;     //species the offset in bytes from the
//                      //bitmapfileheader to the bitmap bits
//  }BITMAPFILEHEADER;
//
//typedef  struct  tagBITMAPINFOHEADER
//  {
//  DWORD biSize;          //specifies the number of bytes required by the struct
//  LONG  biWidth;         //specifies width in pixels
//  LONG  biHeight;        //species height in pixels
//  WORD  biPlanes;        //specifies the number of color planes, must be 1
//  WORD  biBitCount;      //specifies the number of bit per pixel
//  DWORD biCompression;   //spcifies the type of compression
//  DWORD biSizeImage;     //size of image in bytes
//  LONG  biXPelsPerMeter; //number of pixels per meter in x axis
//  LONG  biYPelsPerMeter; //number of pixels per meter in y axis
//  DWORD biClrUsed;       //number of colors used by the bitmap
//  DWORD biClrImportant;  //number of colors that are important
//  }BITMAPINFOHEADER;
//
//===========================================================================
uint8_t *LoadBitmapFile(wchar_t *filename, BITMAPINFOHEADER *bitmapInfoHeader)
  {
  FILE
   *filePtr;
  BITMAPFILEHEADER
    bitmapFileHeader;
  //We will allocate memory for the bitmap, image data at this pointer.
  uint8_t
   *bitmapImage;
  size_t
    bytes_read_from_bitmap;
  errno_t
    file_open_err;

  //open filename in read binary mode
  file_open_err=_wfopen_s(&filePtr,filename,L"rb");
  if(file_open_err)
    {
    wprintf(L"Error# %d: %s reading the BMP in: \"%s\"\n",file_open_err,strerror(file_open_err), filename);
    return(NULL);
    }

  //read the bitmap file header
  fread(&bitmapFileHeader,sizeof(BITMAPFILEHEADER),1,filePtr);

  //verify that this is a bmp file by check bitmap id
  if(0x4D42 != bitmapFileHeader.bfType)
    {
    fclose(filePtr);
    return(NULL);
    }

  //read the bitmap info header
  fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER),1,filePtr);

  //move file point to the begining of bitmap data
  fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

  //allocate enough memory for the bitmap image data
  bitmapImage = (uint8_t*)malloc(bitmapInfoHeader->biSizeImage);

  //verify memory allocation
  if(NULL == bitmapImage)
    {
    fclose(filePtr);
    return(NULL);
    }

  //read in the bitmap image data
  bytes_read_from_bitmap=
    fread(bitmapImage,1,bitmapInfoHeader->biSizeImage,filePtr);

  //make sure bitmap image data was read
  if(bytes_read_from_bitmap  != bitmapInfoHeader->biSizeImage)
    {
    free(bitmapImage);
    fclose(filePtr);
    return(NULL);
    }

  //close file
  fclose(filePtr);
  //return bitmap image data
  return(bitmapImage);
  }
//===========================================================================
uint16_t Get_565_from_RGB(uint8_t r,uint8_t g,uint8_t b)
  {
  uint16_t
    pixel_565;

#if 0
  //simple shift truncate
  //The display takes two bytes (565) RRRRRGGG|GGGBBBBB|  to show one pixel.
  //                                  rrrrrrrr|        |     <<8
  //                                       ggg|ggggg   |     << 3
  //                                          |   bbbbb|bbb  >> 3

  //straight forward, truncates
  pixel_565=(((WORD)(r & 0xF8))<<8) |
            (((WORD)(g & 0xFC))<<3) |
            (((WORD)(b & 0xF8))>>3);

  // From web:
  // rgb565 = (r>>3)<<11 + (g>>2)<<5 + (b>>3)

#else

  //Then this:
  //https://www.lucasgaland.com/24-bit-16-bit-color-converter-tool-for-embedded-lcd-guis/
  //
  // R5 = (R8 * 249 + 1014) >> 11;
  // G6 = (G8 * 253 + 505) >> 10;
  // B5 = (B8 * 249 + 1014) >> 11;
  //
  //It is pretty close to the shift, but nudges the edges out a bit.
  uint8_t
    R5;
  uint8_t
    G6;
  uint8_t
    B5;
  R5=(uint8_t)((((double)r*(double)249.0)+(double)1014.0) / (double)2048.0);
  G6=(uint8_t)((((double)g*(double)253.0)+(double) 505.0) / (double)1024.0);
  B5=(uint8_t)((((double)b*(double)249.0)+(double)1014.0) / (double)2048.0);
  //The display takes two bytes (565) RRRRRGGG|GGGBBBBB|  to show one pixel.
  //                                  rrrrr   |        |     << 11
  //                                       ggg|ggg     |     << 5
  //                                          |   bbbbb|     << 0

  pixel_565=((WORD)(R5)<<11) |
            ((WORD)(G6)<<5) |
             (WORD)(B5);

#endif

  //High low byte order is flipped. Sigh.
  uint16_t
    pixel_swap_565;
  pixel_swap_565=((pixel_565&0xFF00) >> 8 ) | ((pixel_565&0x00FF) << 8 );
  return(pixel_565);
  }
//===========================================================================
void usage(wchar_t *our_name)
  {
  wprintf(L"Usage:\n");
  wprintf(L"  %s [infile] [outfile]\n", our_name);
  wprintf(L"  [infile] is 24-bit BMP\n");
  wprintf(L"  [outfile] is binary R5G6B6 file\n");
  }
//===========================================================================
int _tmain(int argc, _TCHAR*  argv[])
  {
  BITMAPINFOHEADER
    bitmapInfoHeader;
  uint8_t
   *bitmapData;

  //Must have 3 parameters
  if(3 != argc)
    {
    usage(argv[0]);
    return(1);
    }

  wprintf(L"Reading input bitmap: %s\n",argv[1]);
  //Crack open the BMP, and read the image data
  bitmapData = LoadBitmapFile(argv[1],&bitmapInfoHeader);

  //Check that the operation went OK.
  if(NULL == bitmapData)
    {
    wprintf(L"Error reading/processing the BMP.\n");
    usage(argv[0]);
    return(1);
    }

  wprintf(L"biWidth = %d\n",bitmapInfoHeader.biWidth);
  wprintf(L"biHeight = %d\n",bitmapInfoHeader.biHeight);
  wprintf(L"biBitCount = %d\n",bitmapInfoHeader.biBitCount);

  //We are only set up to do 24 bit BMPs, which are
  //more or less standard.
  if(24  != bitmapInfoHeader.biBitCount)
    {
    wprintf(L"This program can only process 24-bit BMPs, the input BMP is %d\n",
            bitmapInfoHeader.biBitCount);
    usage(argv[0]);
    //free  the  memory  allocated  by  LoadBitmapFile
    free(bitmapData);
    return(1);
    }

  errno_t
    file_open_err;
  FILE
   *Out_565_data;

  file_open_err=_wfopen_s(&Out_565_data,argv[2],L"wb");
  if(file_open_err)
    {
    wprintf(L"Error# %d: %s opening the binary R5G6B5 output file: \"%s\"\n",file_open_err,strerror(file_open_err), argv[2]);
    usage(argv[0]);
    //free the memory allocated by LoadBitmapFile
    free(bitmapData);
    //Indicate failure, exit
    return(NULL);
    }
  wprintf(L"Opened \"%s\" as the binary R5G6B5 output file.\n",argv[2]);

  //The output file is open. Write out the header information to the file.

  //Now we just loop down the file, line by line (bottom first),
  //create the 16-bit words and spool them out.
  int
    col;
  int
    row;

  //Our pointer into the bitmap data
  uint8_t
   *data_pointer;

  //The width of a line, in bytes, must be a multiple of 4;
  int
    line_width;

  //3 bytes per pixel, round up to next 4-byte alignment
  line_width=((3*bitmapInfoHeader.biWidth)+3) & ~0x03;


  //For the FT8xx, there is room for an 800x480 image in 
  //RAM_G, but the maximum bitmap is 512x512.
  //The logical split is 400x480 
  //So we need to do the left half, then the right half.
  //This is equivalent to splitting the file using an image
  //editing program, converting each half, then appending
  //the two halves into one file.

  if((511!=bitmapInfoHeader.biWidth)||
     (300!=bitmapInfoHeader.biHeight))
    {
    wprintf(L"Can only split 511x300 files.\n");
    exit(0);
    }

  //Upper Left Quarter
  for(row=0;row<bitmapInfoHeader.biHeight;row++)
    {
    //Point to the left half of the current row in the bitmap
    //data (starting at the bottom)
    data_pointer=bitmapData +
                 (bitmapInfoHeader.biHeight-1-row)*line_width;
    //work across the row.
    for(col=0;col<bitmapInfoHeader.biWidth;col++)
      {
      uint8_t
        r;
      uint8_t
        g;
      uint8_t
        b;
      //pull the pixel out of the stream
      b = *data_pointer++;
      g = *data_pointer++;
      r = *data_pointer++;
      //Convert to 565
      WORD
        pixel_565;
      pixel_565=Get_565_from_RGB(r,g,b);

      //Write one pixel's worth / 2 bytes of data
      fwrite((void *)&pixel_565,2,1,Out_565_data);
      }
    }
/*
  //Upper Right Quarter
  for(row=0;row<bitmapInfoHeader.biHeight/2;row++)
    {
    //Point to the right half of the current row in the
    //bitmap data (starting at the bottom)
    data_pointer=bitmapData +
                 (bitmapInfoHeader.biHeight-1-row)*line_width+
                 line_width/2;
    //work across the row.
    for(col=0;col<bitmapInfoHeader.biWidth/2;col++)
      {
      uint8_t
        r;
      uint8_t
        g;
      uint8_t
        b;
      //pull the pixel out of the stream
      b = *data_pointer++;
      g = *data_pointer++;
      r = *data_pointer++;
      //Convert to 565
      WORD
        pixel_565;
      pixel_565=Get_565_from_RGB(r,g,b);

      //Write one pixel's worth / 2 bytes of data
      fwrite((void *)&pixel_565,2,1,Out_565_data);
      }
    }

  //Lower Left Quarter
  for(row=bitmapInfoHeader.biHeight/2;row<bitmapInfoHeader.biHeight;row++)
    {
    //Point to the left half of the current row in the bitmap
    //data (starting at the bottom)
    data_pointer=bitmapData +
                 (bitmapInfoHeader.biHeight-1-row)*line_width;
    //work across the row.
    for(col=0;col<bitmapInfoHeader.biWidth/2;col++)
      {
      uint8_t
        r;
      uint8_t
        g;
      uint8_t
        b;
      //pull the pixel out of the stream
      b = *data_pointer++;
      g = *data_pointer++;
      r = *data_pointer++;
      //Convert to 565
      WORD
        pixel_565;
      pixel_565=Get_565_from_RGB(r,g,b);

      //Write one pixel's worth / 2 bytes of data
      fwrite((void *)&pixel_565,2,1,Out_565_data);
      }
    }

  //Lower Right Quarter
  for(row=bitmapInfoHeader.biHeight/2;row<bitmapInfoHeader.biHeight;row++)
    {
    //Point to the right half of the current row in the
    //bitmap data (starting at the bottom)
    data_pointer=bitmapData +
                 (bitmapInfoHeader.biHeight-1-row)*line_width+
                 line_width/2;
    //work across the row.
    for(col=0;col<bitmapInfoHeader.biWidth/2;col++)
      {
      uint8_t
        r;
      uint8_t
        g;
      uint8_t
        b;
      //pull the pixel out of the stream
      b = *data_pointer++;
      g = *data_pointer++;
      r = *data_pointer++;
      //Convert to 565
      WORD
        pixel_565;
      pixel_565=Get_565_from_RGB(r,g,b);

      //Write one pixel's worth / 2 bytes of data
      fwrite((void *)&pixel_565,2,1,Out_565_data);
      }
    }
*/

  //Done with the output file.
  fclose(Out_565_data);

  //free the memory allocated by LoadBitmapFile
  free(bitmapData);

  wprintf(L"Complete.\n");

#if 1 //Create validation BMP from RAW data
  //Header copied form a vaild bitmap
  static const uint8_t bmp_header[54]=
    {0x42,0x4D,0x38,0x94,0x11,0x00,0x00,0x00,0x00,0x00,0x36,0x00,0x00,0x00,0x28,0x00,
     0x00,0x00,0x20,0x03,0x00,0x00,0xE0,0x01,0x00,0x00,0x01,0x00,0x18,0x00,0x00,0x00,
     0x00,0x00,0x02,0x94,0x11,0x00,0x12,0x0B,0x00,0x00,0x12,0x0B,0x00,0x00,0x00,0x00,
     0x00,0x00,0x00,0x00,0x00,0x00};

  //re-open output fil as input file for reading.
  FILE
   *In_565_data;
  file_open_err=_wfopen_s(&In_565_data,argv[2],L"rb");
  if(file_open_err)
    {
    wprintf(L"Readback: Error# %d: %s opening the binary R5G6B5 input file: \"%s\"\n",file_open_err,strerror(file_open_err), argv[2]);
    //Indicate failure, exit
    return(NULL);
    }

  //open an putput file to put the BMP into
  FILE
   *Out_BMP;
  file_open_err=_wfopen_s(&Out_BMP,L"bmp_check.bmp",L"wb");
  if(file_open_err)
    {
    wprintf(L"Readback: Error# %d: %s opening the binary BMP output file: \"%s\"\n",file_open_err,strerror(file_open_err), argv[2]);
    //Indicate failure, exit
    return(NULL);
    }

  //Write the BMP header
  fwrite((void *)&bmp_header,54,1,Out_BMP);

  //move the pixels over
  for(int32_t line=479;0 <= line;line--)
    {
    //Seek to the correct location in the RAW file for this line's left half
    fseek(In_565_data,line*400*2,SEEK_SET);
   //Loop through left pixels
    for(int32_t col=0;col < 400;col++)
      {
      // ? reading as a word avoids need to swap ?
      WORD
        pixel_565;
      fread((void *)&pixel_565,2,1,In_565_data);

      //The display takes two bytes (565) RRRRRGGG|GGGBBBBB|  to show one pixel.
      //                                  rrrrr   |        |     << 11
      //                                       ggg|ggg     |     << 5
      //                                          |   bbbbb|     << 0

      //Extract the sub-pixels
      uint8_t
        r;
      uint8_t
        g;
      uint8_t
        b;

      b=(pixel_565&0x001F) << 3;
      g=(pixel_565&0x07E0) >> 3;
      r=(pixel_565&0xF800) >> 8;

      //Write this pixel to the BMP
      fwrite((void *)&b,1,1,Out_BMP);
      fwrite((void *)&g,1,1,Out_BMP);
      fwrite((void *)&r,1,1,Out_BMP);

      }

    //Seek to the correct location in the RAW file for this line's right half
    fseek(In_565_data,line*400*2 + 480*400*2,SEEK_SET);
   //Loop through right pixels
    for(int32_t col=0;col < 400;col++)
      {
      // ? reading as a word avoids need to swap ?
      WORD
        pixel_565;
      fread((void *)&pixel_565,2,1,In_565_data);

      //The display takes two bytes (565) RRRRRGGG|GGGBBBBB|  to show one pixel.
      //                                  rrrrr   |        |     << 11
      //                                       ggg|ggg     |     << 5
      //                                          |   bbbbb|     << 0

      //Extract the sub-pixels
      uint8_t
        r;
      uint8_t
        g;
      uint8_t
        b;

      b=(pixel_565&0x001F) << 3;
      g=(pixel_565&0x07E0) >> 3;
      r=(pixel_565&0xF800) >> 8;

      //Write this pixel to the BMP
      fwrite((void *)&b,1,1,Out_BMP);
      fwrite((void *)&g,1,1,Out_BMP); 
      fwrite((void *)&r,1,1,Out_BMP);

      }



    }
  fclose(Out_BMP);
  fclose(In_565_data);


#endif //Create validation BMP from RAW

  //Indicate success, exit
  return(0);
  }
//===========================================================================
