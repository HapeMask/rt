#include <QImage>
#include <QObject>

#include "texture.hpp"
#include "color/color.hpp"
#include "utility.hpp"

#include <cmath>
#include <iostream>

using std::cerr;
using std::endl;

#ifdef RT_MULTITHREADED
bool texture2D::lookupInitialized;
rgbColor** texture2D::lookupCache;
#endif

rgbColor texture2D::lookup(const vec2& uv) const {
    // Use bilinear interpolation. TODO: Maybe use the GPU for this
    // somehow? It does have dedicated bilerp hardware...
    const float x = uv.x() * (width-1);
    const float y = uv.y() * (height-1);

    const int xMin = floor(x);
    const int xMax = ceil(x);

    const int yMin = floor(y);
    const int yMax = ceil(y);

    const rgbColor top = lerp(
            _lookup(xMin, yMax),
            _lookup(xMax, yMax),
            x - xMin);

    const rgbColor bot = lerp(
            _lookup(xMin, yMin),
            _lookup(xMax, yMin),
            x - xMin);

    return lerp(bot, top, y - yMin);
}

colorTexture2D::colorTexture2D(const int& w, const int& h) : texture2D(w, h) {
    texels = new rgbColor*[height];
    for(int i=0; i < height; ++i){
        texels[i] = new rgbColor[width];
        for(int j=0; j < width; ++j){
            texels[i][j] = rgbColor(0);
        }
    }
}

colorTexture2D::colorTexture2D(const string& filename) :
    texture2D(0, 0), texels(NULL) {

    QImage img(QObject::tr(filename.c_str()));
    if(img.isNull()){
        cerr << "Error loading texture image: " << filename << endl;
        return;
    }

    width = img.width();
    height = img.height();

    const float inverseGamma = 2.2f;

    texels = new rgbColor*[height];
    for(int i=0; i < height; ++i){
        texels[i] = new rgbColor[width];
        for(int j=0; j < width; ++j){
            const QRgb c = img.pixel(j, i);

            // Un-apply gamma correction to textures since we apply it after rendering
            texels[i][j] = rgbColor(
                    powf(qRed(c) / 255.f, inverseGamma),
                    powf(qGreen(c) / 255.f, inverseGamma),
                    powf(qBlue(c) / 255.f, inverseGamma)
                    );
        }
    }
}

const rgbColor& colorTexture2D::_lookup(const int& x, const int& y) const {
    return texels[y][x];
}
