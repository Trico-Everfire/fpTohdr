#include <cmath>
#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <cstring>
#include "filesystem.h"

#define VALIDATOR 19871983

bool CFIleSystem::writeHDR(const char *path, const CFIleSystem::FP32HDRHandle &handle) {
    if(handle.validator != VALIDATOR)
        return false;

    std::string fileContents = {"#?RADIANCE\n"};

    fileContents.append("FORMAT=");
    fileContents.append(handle.format == RLE_RGBE ? "32-bit_rle_rgbe" : "32-bit_rle_xyze");
    fileContents.append("\n");

    for(const auto& data : handle.metadata)
    {
        fileContents.append(data.first);
        fileContents.append("=");
        fileContents.append(data.second);
        fileContents.append("\n");
    }

    fileContents.append("\n");

    if(handle.flipHorizontal)
    {
        fileContents.append("-Y ");
        fileContents.append(std::to_string(handle.width));

        fileContents.append(" ");

        fileContents.append("+X ");
        fileContents.append(std::to_string(handle.height));
    }
    else
    {
        fileContents.append("+Y ");
        fileContents.append(std::to_string(handle.height));

        fileContents.append(" ");

        fileContents.append("-X ");
        fileContents.append(std::to_string(handle.width));
    }

    fileContents.append("\n");

    auto sl = handle.imageData;

    if( handle.flipVertical ) std::reverse(sl.begin(), sl.end());

    for( const auto& data : sl )
    {
        fileContents.append(1, static_cast<const char>(data.r));
        fileContents.append(1, static_cast<const char>(data.g));
        fileContents.append(1, static_cast<const char>(data.b));
        fileContents.append(1, static_cast<const char>(data.exponent));
    }

    std::ofstream myfile;
    myfile.open (path);
    myfile << fileContents;
    myfile.close();

    return true;

}

void CFIleSystem::addMetadata(CFIleSystem::FP32HDRHandle& handle, const char *&key, const char *&value) {

    if(handle.validator != VALIDATOR)
        return;

    handle.metadata[key] = value;
}

CFIleSystem::FP32HDRHandle
CFIleSystem::createHandle(Format format, unsigned int width, unsigned int height, const float *red,
                          const float *green, const float *blue) {


    FP32HDRHandle handle;
    handle.width = width;
    handle.height = height;
    handle.validator = VALIDATOR;
    handle.format = format;
    handle.flipHorizontal = false;
    handle.flipVertical = false;
    handle.imageData = {};

    for(int i = 0; i < width * height; i++)
    {
        PixFormat form = {};

        float v;
        int e;

        v = red[i];
        if (green[i] > v) v = green[i];
        if (blue[i] > v) v = blue[i];
        if (v < 1e-32) {
            form.r = form.g = form.b = form.exponent = static_cast<std::byte>(0);
        } else {
            v = frexp(v, &e) * 256.0 / v;
            form.r = static_cast<std::byte>(red[i] * v);
            form.g = static_cast<std::byte>(green[i] * v);
            form.b = static_cast<std::byte>(blue[i] * v);
            form.exponent = static_cast<std::byte>(e + 128);
        }

        handle.imageData.push_back(form);
    }



    return handle;
}

void CFIleSystem::flipHorizontal(CFIleSystem::FP32HDRHandle &handle, bool flip)
{
    if(handle.validator != VALIDATOR)
        return;
    handle.flipHorizontal = flip;
}

void CFIleSystem::flipVertical(CFIleSystem::FP32HDRHandle &handle, bool flip) {
    if(handle.validator != VALIDATOR)
        return;
    handle.flipVertical = flip;
}

void CFIleSystem::rotate90Degrees(CFIleSystem::FP32HDRHandle &handle)
{

    int size = sqrt(handle.width * handle.height);
    auto result = std::vector<PixFormat>(handle.width * handle.height);

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            result[i * size + j] = handle.imageData[(size - j - 1) * size + i];
        }
    }

    handle.imageData.clear();

    for(int i = 0; i < handle.width * handle.height; i++)
    {
        handle.imageData.push_back(result[i]);
    }

    CFIleSystem::flipHorizontal(handle, true);

//    uint width = handle.width;
//    uint height = handle.height;
//
//    handle.width = height;
//    handle.height = width;



}
