#include <Gdiplus.h>

class CustomImage
{
public:
    int Id;                  
    CString Name;       
    Gdiplus::Bitmap* Bitmap;
    int Width;             
    int Height;             
    CString PixelFormat;   
    CString Path;   

    CustomImage() : Bitmap(nullptr) {};
    CustomImage(int id, CString name, Gdiplus::Bitmap* bitmap, int width, int height, CString pixelFormat, CString path)
        : Id(id), Name(name), Bitmap(bitmap), Width(width), Height(height), PixelFormat(pixelFormat), Path(path) {}

};