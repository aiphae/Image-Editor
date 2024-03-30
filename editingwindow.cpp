#include <QFileDialog>
#include <QScreen>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QMessageBox>

#include "editingwindow.h"
#include "ui_editingwindow.h"
#include "image.h"

const double WIDTH_PERCENT = 0.7;
const double HEIGHT_PERCENT = 0.8;

EditingWindow::EditingWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EditingWindow)
{
    ui->setupUi(this);

    // Get desktop size
    QScreen *desktop = QGuiApplication::primaryScreen();
    QRect desktopRect = desktop->availableGeometry();
    int desktopWidth = desktopRect.width();
    int desktopHeight = desktopRect.height();

    // Make the window size relative to the desktop's
    resize(desktopWidth * WIDTH_PERCENT, desktopHeight * HEIGHT_PERCENT);

    // Connect the brightness slider
    connect(ui->brighntess_slider, &QSlider::valueChanged, this, [=](){
        src_image.current_brightness = ui->brighntess_slider->value();
        apply_changes();
    });

    // Connect the contrast slider
    connect(ui->contrast_slider, &QSlider::valueChanged, this, [=](){
        src_image.current_contrast = ui->contrast_slider->value();
        apply_changes();
    });

    // Connect the saturation slider
    connect(ui->saturation_slider, &QSlider::valueChanged, this, [=](){
        src_image.current_saturation = ui->saturation_slider->value();
        apply_changes();
    });

    // Connect the R slider
    connect(ui->red_slider, &QSlider::valueChanged, this, [=](){
        src_image.current_red = ui->red_slider->value();
        apply_changes();
    });

    // Connect the G slider
    connect(ui->green_slider, &QSlider::valueChanged, this, [=](){
        src_image.current_green = ui->green_slider->value();
        apply_changes();
    });

    // Connect the B slider
    connect(ui->blue_slider, &QSlider::valueChanged, this, [=](){
        src_image.current_blue = ui->blue_slider->value();
        apply_changes();
    });

    // Connect none filter
    connect(ui->none_button, &QPushButton::clicked, this, [=](){
        if (src_image.filter != "none") {
            src_image.filter = "none";
            apply_filter();
            apply_changes();
        }
    });

    // Connect blur filter
    connect(ui->blur_button, &QPushButton::clicked, this, [=](){
        if (src_image.filter != "blur") {
            src_image.filter = "blur";
            apply_filter();
            apply_changes();
        }
    });

    // Connect grayscale filter
    connect(ui->grayscale_button, &QPushButton::clicked, this, [=](){
        if (src_image.filter != "grayscale") {
            src_image.filter = "grayscale";
            apply_filter();
            apply_changes();
        }
    });

    // Connect sepia filter
    connect(ui->sepia_button, &QPushButton::clicked, this, [=](){
        if (src_image.filter != "sepia") {
            src_image.filter = "sepia";
            apply_filter();
            apply_changes();
        }
    });

    // Connect edge detection filter
    connect(ui->edge_detection_button, &QPushButton::clicked, this, [=](){
        if (src_image.filter != "edge") {
            src_image.filter = "edge";
            apply_filter();
            apply_changes();
        }
    });

    // Connect inverse filter
    connect(ui->inverse_button, &QPushButton::clicked, this, [=](){
        if (src_image.filter != "inverse") {
            src_image.filter = "inverse";
            apply_filter();
            apply_changes();
        }
    });

    // Connect emboss filter
    connect(ui->emboss_button, &QPushButton::clicked, this, [=](){
        if (src_image.filter != "emboss") {
            src_image.filter = "emboss";
            apply_filter();
            apply_changes();
        }
    });

    display_image = nullptr;
}

void EditingWindow::displayImage(const QPixmap &image) {
    scene.clear();
    display_image = scene.addPixmap(image);
    scene.setSceneRect(0, 0, image.width(), image.height());
    ui->imageView->setScene(&scene);
    ui->imageView->show();
}

// Sets the image being edited from the main window
void EditingWindow::setImage(const Image image)
{
    src_image = image;
    apply_changes();
    ui->filename_label->setText(src_image.filename);
}

// Applies ALL changes
void EditingWindow::apply_changes() {
    apply_adjustments();
    displayImage(src_image.temp_img);
}

void EditingWindow::apply_filter() {
    if (src_image.filter == "none") {
        src_image.apply_none();
    }
    else if (src_image.filter == "grayscale") {
        src_image.apply_grayscale();
    }
    else if (src_image.filter == "sepia") {
        src_image.apply_sepia();
    }
    else if (src_image.filter == "edge") {
        src_image.apply_edge_detection();
    }
    else if (src_image.filter == "inverse") {
        src_image.apply_inverse();
    }
    else if (src_image.filter == "emboss") {
        src_image.apply_emboss();
    }
    else if (src_image.filter == "blur") {
        src_image.apply_blur();
    }
}

void EditingWindow::apply_adjustments() {
    src_image.adjust_brightness();
    src_image.adjust_contrast();
    src_image.adjust_saturation();
    src_image.adjust_rgb();

    displayImage(src_image.temp_img);
}

void EditingWindow::on_another_image_button_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open image", "C://Users//Public//Pictures", "Images (*.png *.jpeg *.jpg)");

    // If no file was selected
    if (filename.isEmpty()) return;

    // If file does not exist
    if (!QFile(filename).exists()) {
        QMessageBox::warning(this, "Error", "File does not exist.");
        return;
    }

    QPixmap new_image(filename);
    QFileInfo fileInfo(filename);
    QString pure_filename = fileInfo.fileName();

    Image new_img(new_image, pure_filename);
    src_image = new_img;

    reset_sliders();

    ui->filename_label->setText(src_image.filename);

    apply_changes();
}

void EditingWindow::reset_sliders() {
    ui->brighntess_slider->setValue(0);
    ui->contrast_slider->setValue(0);
    ui->saturation_slider->setValue(0);
    ui->red_slider->setValue(0);
    ui->blue_slider->setValue(0);
    ui->green_slider->setValue(0);
}

void EditingWindow::on_apply_changes_button_clicked()
{
    src_image.src_img = src_image.temp_img;
    src_image.filtered_img = src_image.temp_img;
    reset_sliders();
    src_image.filter = "none";
}

void EditingWindow::on_save_as_button_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save as...", "C://Users//Public//Pictures", "*.png");
    if (filename.isEmpty()) return;
    if (!src_image.temp_img.isNull()) {
        if (!src_image.temp_img.save(filename)) {
            QMessageBox::warning(this, "Error", "Failed to save the image.");
        }
    }
}

EditingWindow::~EditingWindow()
{
    delete display_image;
    delete ui;
}
