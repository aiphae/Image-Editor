#ifndef IMAGE_H
#define IMAGE_H

#include <QPixmap>
#include <QString>
#include <QImage>

class Image
{
public:
    Image();
    Image(QPixmap &src_img, QString &filename) :
        src_img(src_img),
        temp_img(src_img),
        filtered_img(src_img),
        filename(filename)
        {};

    // Adjustments
    void adjust_brightness();
    int current_brightness = 0;

    void adjust_contrast();
    int current_contrast = 0;

    void adjust_saturation();
    int current_saturation = 0;

    int current_red = 0;
    int current_green = 0;
    int current_blue = 0;
    void adjust_rgb();

    // Filters
    void apply_none();
    void apply_grayscale();
    void apply_sepia();
    void apply_blur();
    void apply_edge_detection();
    void apply_inverse();
    void apply_emboss();

    // Chosen filter
    QString filter = "none";

    // Source image
    QPixmap src_img;

    // Image to apply adjustments and filter
    QPixmap temp_img;

    // Image with applied filter to enhance performance
    QPixmap filtered_img;

    // Name of the file
    QString filename;
};

#endif // IMAGE_H
