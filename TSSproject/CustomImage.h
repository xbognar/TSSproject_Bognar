#include <vector>
class CustomImage
{
public:
    int Id;                  // Image ID (for internal use)
    CString Name;             // Image name
    std::vector<BYTE> ImageData;  // Binary data for the image
    int Width;                // Image width
    int Height;               // Image height
    CString PixelFormat;      // Pixel format (e.g., RGB, RGBA)
    CString Path;             // Path to the image file

    CustomImage() {};
    CustomImage(int id, CString name, std::vector<BYTE> data, int width, int height, CString pixelFormat, CString path)
        : Id(id), Name(name), ImageData(data), Width(width), Height(height), PixelFormat(pixelFormat), Path(path) {}
};
