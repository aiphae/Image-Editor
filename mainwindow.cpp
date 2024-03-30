#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "editingwindow.h"
#include <QScreen>
#include <QFileDialog>
#include <QFileInfo>
#include <QPixmap>
#include "image.h"

const double WIDTH_PERCENT = 0.4;
const double HEIGHT_PERCENT = 0.4;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Get desktop width and height
    QScreen *desktop = QGuiApplication::primaryScreen();
    QRect desktopRect = desktop->availableGeometry();
    int desktopWidth = desktopRect.width();
    int desktopHeight = desktopRect.height();

    // Make the window size relative to the desktop
    resize(desktopWidth * WIDTH_PERCENT, desktopHeight * HEIGHT_PERCENT);
}

void MainWindow::on_upload_image_button_clicked()
{
    // Get an image
    QString filename = QFileDialog::getOpenFileName(this, "Open image", "C://Users//Public//Pictures", "Images (*.png *.jpeg *.jpg)");
    QPixmap src_image(filename);
    QFileInfo fileInfo(filename);
    QString pure_filename = fileInfo.fileName();
    Image src(src_image, pure_filename);

    // Pass the image to the editing window
    EditingWindow *editingWindow = new EditingWindow();
    editingWindow->setImage(src);

    close();

    editingWindow->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
