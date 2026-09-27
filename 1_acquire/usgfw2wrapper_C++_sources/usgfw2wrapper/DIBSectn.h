/* **********************************************************************
DIBSectn.H  Header file for sample source to demonstrate the use 
            of DIBSections

  Contains the following functions for Creating/Saving/Drawing:
    DSCreateDIBSection()
    DSLoadDIBSectionFromBMPFile()
    DSDrawDIBSectionOnDC()
    DSStoreDIBSectionInBMPFile()

  Contains the following helper functions:
    DSCreatePaletteForDIBSection()
    DSGetBITMAPINFOForDIBSection()
    DSColorTableSize()
    DSTotalBytesSize()
    DSImageBitsSize()
    DSGetPointerToDIBSectionImageBits()
    DSCreateSpectrumPalette() 
	DSBitsperPixel()
	DSGetDIBSectionFromClipboard()
	DSPutDIBSectionOnClipboard()
	DSCopyBitmap();

  Notes:
    * Most of the functions in this module which are passed an HBITMAP
      as a parameter assume the HBITMAP is not selected into a device
      context at the time of the call. Since the underlying APIs usually
      require this, so do these functions.
    * Throughout this module, code is in place to handle color tables
      in DIBSections (in the BITMAPINFO) even when it is not necessary.
      This helps the re-usability of the code, since it is often desirable
      to include a "suggested" color table when storing 16bpp or higher
      BMP files.

********************************************************************** */

// Create/Load/Store/Draw functions
//HBITMAP DSCreateDIBSection( DWORD dwX, DWORD dwY, WORD wBits );
HBITMAP DSCreateDIBSection(DWORD dwX, DWORD dwY, WORD wBits, RGBQUAD* colors, int num_colors);
BOOL DSLoadDIBSectionFromBMPFile( LPTSTR szFileName, HBITMAP *phBitmap, HPALETTE *phPalette );
BOOL DSDrawDIBSectionOnDC( HDC hDC, HBITMAP hBitmap, LPRECT pRect );
BOOL DSStoreDIBSectionInBMPFile( LPTSTR szFileName, HBITMAP hBitmap );

// Helper Functions
HPALETTE DSCreatePaletteForDIBSection( HBITMAP hBitmap );
LPBITMAPINFO DSGetBITMAPINFOForDIBSection( HBITMAP hBitmap );
DWORD DSColorTableSize( LPBITMAPINFO pbmi );
DWORD DSTotalBytesSize( LPBITMAPINFO pbmi );
DWORD DSImageBitsSize( LPBITMAPINFO pbmi );
LPBYTE DSGetPointerToDIBSectionImageBits( HBITMAP hBitmap );
HPALETTE DSCreateSpectrumPalette( void );
DWORD DSBitsPerPixel (HBITMAP );
BOOL DSGetOptimalDIBFormat(HDC, BITMAPINFOHEADER*);
BOOL DSGetRGBBitsPerPixel(HDC, PINT, PINT, PINT);
HBITMAP DSGetDIBSectionFromClipboard( void );
BOOL DSPutDIBSectionOnClipboard(HBITMAP);
HBITMAP DSCopyBitmap(HBITMAP, HPALETTE);
HBITMAP DSCopyBitmapEx(HBITMAP, HPALETTE, WORD);
HGLOBAL DSGetDIBFromDIBSection(HBITMAP);

#define BYTESPERLINE(Width, BPP) ((WORD)((((DWORD)(Width) * (DWORD)(BPP) + 31) >> 5)) << 2)
#define NEW_DIB_FORMAT(lpbih) (lpbih->biSize != sizeof(BITMAPCOREHEADER))

/* Macro to swap two values */
#define SWAP(x, y)   ((x)^=(y)^=(x)^=(y))

/* **********************************************************************
   Sample declares by Fred Nava for use from VB 

Type BITMAPINFOHEADER
        biSize As Long
        biWidth As Long
        biHeight As Long
        biPlanes As Integer
        biBitCount As Integer
        biCompression As Long
        biSizeImage As Long
        biXPelsPerMeter As Long
        biYPelsPerMeter As Long
        biClrUsed As Long
        biClrImportant As Long
End Type

Type RGBQUAD
        rgbBlue As Byte
        rgbGreen As Byte
        rgbRed As Byte
        rgbReserved As Byte
End Type

Type RECT
        Left As Long
        Top As Long
        Right As Long
        Bottom As Long
End Type

Type BITMAPINFO
        bmiHeader As BITMAPINFOHEADER
        bmiColors As RGBQUAD
End Type

Declare Function DSCreateDIBSection Lib "dibsectn.dll" ( _
    ByVal dwX As Long, _
    ByVal dwY As Long, _
    ByVal wBits As Integer) _
As Long

Declare Function DSLoadDIBSectionFromBMPFile Lib "dibsectn.dll" ( _
    ByVal szFileName As String, _
    phBitmap As Long, _
    phPalette As Long) _
As Boolean

Declare Function DSDrawDIBSectionOnDC Lib "dibsectn.dll" ( _
    ByVal hDC As Long, _
    ByVal hBitmap As Long, _
    pRect As RECT) _
As Boolean

Declare Function DSStoreDIBSectionInBMPFile Lib "dibsectn.dll" ( _
    ByVal szFileName As String, _
    ByVal hBitmap As Long) _
As Boolean

Declare Function DSCreatePaletteForDIBSection Lib "dibsectn.dll" ( _
    ByVal hBitmap As Long) _
As Long

Declare Function DSGetBITMAPINFOForDIBSection Lib "dibsectn.dll" ( _
    ByVal hBitmap As Long) _
As Long

Declare Function DSColorTableSize Lib "dibsectn.dll" ( _
    ByVal pbmi As Long) _
As Long

Declare Function DSTotalBytesSize Lib "dibsectn.dll" ( _
    ByVal pbmi As Long) _
As Long

Declare Function DSImageBitsSize Lib "dibsectn.dll" ( _
    ByVal pbmi As Long) _
As Long

Declare Function DSGetPointerToDIBSectionImageBits Lib "dibsectn.dll" ( _
    ByVal hBitmap As Long) _
As Long

Declare Function DSCreateSpectrumPalette Lib "dibsectn.dll" () _
As Long

********************************************************************** */
