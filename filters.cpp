#include <QImage>
#include <QPixmap>
#include <cmath>
#include "image.h"

void Image::apply_none() {
    filtered_img = src_img;
}

void Image::apply_grayscale() {
    QImage img = src_img.toImage();
    QRgb pixel;
    int red, green, blue, avg;
    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
            pixel = img.pixel(x, y);

            red = qRed(pixel);
            green = qGreen(pixel);
            blue = qBlue(pixel);

            avg = (red + green + blue) / 3;

            img.setPixel(x, y, qRgb(avg, avg, avg));
        }
    }

    filtered_img = QPixmap::fromImage(img);
}

void Image::apply_sepia() {
    QImage img = src_img.toImage();
    int width = img.width();
    int height = img.height();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            QRgb pixel = img.pixel(x, y);

            int oldRed = qRed(pixel);
            int oldGreen = qGreen(pixel);
            int oldBlue = qBlue(pixel);

            int newRed = qBound(0, (int)((oldRed * 0.393) + (oldGreen * 0.769) + (oldBlue * 0.189)), 255);
            int newGreen = qBound(0, (int)((oldRed * 0.349) + (oldGreen * 0.686) + (oldBlue * 0.168)), 255);
            int newBlue = qBound(0, (int)((oldRed * 0.272) + (oldGreen * 0.534) + (oldBlue * 0.131)), 255);

            img.setPixel(x, y, qRgb(newRed, newGreen, newBlue));
        }
    }

    filtered_img = QPixmap::fromImage(img);
}

void Image::apply_blur() {
    QImage img = src_img.toImage();
    int width = img.width();
    int height = img.height();

    int radius = 3;

    int kernelSize = radius * 2 + 1;

    for (int y = radius; y < height - radius; ++y) {
        for (int x = radius; x < width - radius; ++x) {
            int sumRed = 0, sumGreen = 0, sumBlue = 0;

            for (int ky = -radius; ky <= radius; ++ky) {
                for (int kx = -radius; kx <= radius; ++kx) {
                    QRgb pixel = img.pixel(x + kx, y + ky);
                    sumRed += qRed(pixel);
                    sumGreen += qGreen(pixel);
                    sumBlue += qBlue(pixel);
                }
            }

            int avgRed = sumRed / (kernelSize * kernelSize);
            int avgGreen = sumGreen / (kernelSize * kernelSize);
            int avgBlue = sumBlue / (kernelSize * kernelSize);

            img.setPixel(x, y, qRgb(avgRed, avgGreen, avgBlue));
        }
    }

    filtered_img = QPixmap::fromImage(img);
}

void Image::apply_edge_detection() {
    QImage img = src_img.toImage();
    int width = img.width();
    int height = img.height();

    int sobelHorizontal[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int sobelVertical[3][3] = {
        {1, 2, 1},
        {0, 0, 0},
        {-1, -2, -1}
    };

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            QRgb pixel = img.pixel(x, y);
            int gray = qGray(pixel);
            img.setPixel(x, y, qRgb(gray, gray, gray));
        }
    }

    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            int gradientX = 0, gradientY = 0;

            for (int j = -1; j <= 1; ++j) {
                for (int i = -1; i <= 1; ++i) {
                    QRgb pixel = img.pixel(x + i, y + j);
                    int intensity = qRed(pixel);

                    gradientX += intensity * sobelHorizontal[j + 1][i + 1];
                    gradientY += intensity * sobelVertical[j + 1][i + 1];
                }
            }

            int gradient = qBound(0, (int) std::sqrt(gradientX * gradientX + gradientY * gradientY), 255);

            img.setPixel(x, y, qRgb(gradient, gradient, gradient));
        }
    }

    filtered_img = QPixmap::fromImage(img);
}

void Image::apply_inverse() {
    QImage img = src_img.toImage();
    int width = img.width();
    int height = img.height();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            QRgb pixel = img.pixel(x, y);

            int invertedRed = 255 - qRed(pixel);
            int invertedGreen = 255 - qGreen(pixel);
            int invertedBlue = 255 - qBlue(pixel);

            img.setPixel(x, y, qRgb(invertedRed, invertedGreen, invertedBlue));
        }
    }

    filtered_img = QPixmap::fromImage(img);
}

void Image::apply_emboss() {
    QImage img = src_img.toImage();
    int width = img.width();
    int height = img.height();

    int embossKernel[3][3] = {
        {-2, -1, 0},
        {-1, 1, 1},
        {0, 1, 2}
    };

    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            int sumRed = 0, sumGreen = 0, sumBlue = 0;

            for (int ky = -1; ky <= 1; ++ky) {
                for (int kx = -1; kx <= 1; ++kx) {
                    QRgb pixel = img.pixel(x + kx, y + ky);
                    int kernelValue = embossKernel[ky + 1][kx + 1];

                    sumRed += qRed(pixel) * kernelValue;
                    sumGreen += qGreen(pixel) * kernelValue;
                    sumBlue += qBlue(pixel) * kernelValue;
                }
            }

            int newRed = qBound(0, sumRed + 128, 255);
            int newGreen = qBound(0, sumGreen + 128, 255);
            int newBlue = qBound(0, sumBlue + 128, 255);

            img.setPixel(x, y, qRgb(newRed, newGreen, newBlue));
        }
    }

    filtered_img = QPixmap::fromImage(img);
}
