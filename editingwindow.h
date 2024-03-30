#ifndef EDITINGWINDOW_H
#define EDITINGWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "image.h"

namespace Ui {
class EditingWindow;
}

class EditingWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditingWindow(QWidget *parent = nullptr);
    ~EditingWindow();
    void setImage(const Image image);
    void displayImage(const QPixmap &image);
    void apply_adjustments();
    void apply_changes();
    void apply_filter();
    void reset_sliders();

signals:
    void imageChanged(const QPixmap& image);

private slots:
    void on_another_image_button_clicked();

    void on_apply_changes_button_clicked();

    void on_save_as_button_clicked();

private:
    Ui::EditingWindow *ui;
    Image src_image;
    QGraphicsScene scene;
    QGraphicsPixmapItem *display_image;
};

#endif // EDITINGWINDOW_H
