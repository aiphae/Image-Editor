#include <QImage>
#include <QPixmap>
#include "image.h"

void Image::adjust_brightness()
{
    QImage img = filtered_img.toImage();
    if (current_brightness != 0) {
        QRgb pixel;
        int red, green, blue;
        for (int y = 0; y < img.height(); ++y) {
            for (int x = 0; x < img.width(); ++x) {
                pixel = img.pixel(x, y);

                red = qRed(pixel);
                green = qGreen(pixel);
                blue = qBlue(pixel);

                red = qBound(0, red + current_brightness, 255);
                green = qBound(0, green + current_brightness, 255);
                blue = qBound(0, blue + current_brightness, 255);

                img.setPixel(x, y, qRgb(red, green, blue));
            }
        }
    }

    temp_img = QPixmap::fromImage(img);
}

void Image::adjust_contrast()
{
    QImage img = temp_img.toImage();
    if (current_contrast != 0) {
        double factor = (259.0 * (current_contrast + 255.0)) / (255.0 * (259.0 - current_contrast));
        QRgb pixel;
        int red, green, blue;
        for (int y = 0; y < img.height(); ++y) {
            for (int x = 0; x < img.width(); ++x) {
                pixel = img.pixel(x, y);

                red = qRed(pixel);
                green = qGreen(pixel);
                blue = qBlue(pixel);

                red = qBound(0, int(factor * (red - 128) + 128), 255);
                green = qBound(0, int(factor * (green - 128) + 128), 255);
                blue = qBound(0, int(factor * (blue - 128) + 128), 255);

                img.setPixel(x, y, qRgb(red, green, blue));
            }
        }
    }

    temp_img = QPixmap::fromImage(img);
}

void Image::adjust_saturation() {
    QImage img = temp_img.toImage();

    if (current_saturation != 0) {
        for (int y = 0; y < img.height(); ++y) {
            for (int x = 0; x < img.width(); ++x) {
                QRgb pixel = img.pixel(x, y);

                int red = qRed(pixel);
                int green = qGreen(pixel);
                int blue = qBlue(pixel);

                int max_color = qMax(red, qMax(green, blue));
                int min_color = qMin(red, qMin(green, blue));

                float avg = (max_color + min_color) / 2.0;

                int saturation = max_color - min_color;

                saturation += current_saturation;

                saturation = qBound(0, saturation, 255);
                if (saturation == 0) {
                    img.setPixel(x, y, qRgb(avg, avg, avg));
                }
                else {
                    float scale = 1.0 + (current_saturation / 255.0) * (saturation > 0 ? 3.0 : -1.0);

                    red = qBound(0, int(avg + scale * (red - avg)), 255);
                    green = qBound(0, int(avg + scale * (green - avg)), 255);
                    blue = qBound(0, int(avg + scale * (blue - avg)), 255);

                    img.setPixel(x, y, qRgb(red, green, blue));
                }
            }
        }
    }

    temp_img = QPixmap::fromImage(img);
}

void Image::adjust_rgb() {
    QImage img = temp_img.toImage();

    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
            QRgb pixel = img.pixel(x, y);

            int red = qRed(pixel);
            int green = qGreen(pixel);
            int blue = qBlue(pixel);

            red = qBound(0, red + current_red, 255);
            green = qBound(0, green + current_green, 255);
            blue = qBound(0, blue + current_blue, 255);

            img.setPixel(x, y, qRgb(red, green, blue));
        }
    }

    temp_img = QPixmap::fromImage(img);
}
