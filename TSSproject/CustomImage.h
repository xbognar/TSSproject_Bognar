#include <Gdiplus.h>

class CustomImage
{
public:
    int Id;                  // Image ID (for internal use)
    CString Name;            // Image name
    Gdiplus::Bitmap* Bitmap; // Bitmap data for the image
    int Width;               // Image width
    int Height;              // Image height
    CString PixelFormat;     // Pixel format (e.g., RGB, RGBA)
    CString Path;            // Path to the image file

    CustomImage() : Bitmap(nullptr) {};
    CustomImage(int id, CString name, Gdiplus::Bitmap* bitmap, int width, int height, CString pixelFormat, CString path)
        : Id(id), Name(name), Bitmap(bitmap), Width(width), Height(height), PixelFormat(pixelFormat), Path(path) {}

};