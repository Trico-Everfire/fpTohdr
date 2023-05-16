#pragma once

#include <cstddef>



class CFIleSystem {

public:
    enum Format
    {
        RLE_RGBE,
        RLE_XYZE
    };

private:

    struct PixFormat
    {
        std::byte r;
        std::byte g;
        std::byte b;
        std::byte exponent;
    };

    struct FP32HDRHandle
    {
        unsigned int validator;
        bool flipHorizontal;
        bool flipVertical;
        Format format;
        std::unordered_map<std::string, std::string> metadata;
        unsigned int width;
        unsigned int height;
        std::vector<PixFormat> imageData;

    };

public:

    static FP32HDRHandle createHandle(Format format, unsigned int width, unsigned int height, const float* red, const float* green, const float* blue);

    static void addMetadata(FP32HDRHandle&, const char* &key, const char* &value);

    static bool writeHDR(const char *path, const CFIleSystem::FP32HDRHandle &handle);

    static void flipHorizontal(CFIleSystem::FP32HDRHandle &handle, bool flip);

    static void flipVertical(CFIleSystem::FP32HDRHandle &handle, bool flip);

    static void rotate90Degrees(CFIleSystem::FP32HDRHandle &handle);

};
