#include "imageFullDialog.h"

ImageFullDialog::ImageFullDialog(QWidget *parent, QPixmap *image_new)
    : QDialog(parent) {
    setModal(false);
    initializeWidgets();
    initializeLayout();
    setWindowTitle(tr("Полный размер"));
    image=image_new;
}

ImageFullDialog::~ImageFullDialog() {
    delete verticalLayout;
}

void ImageFullDialog::initializeWidgets() {
    imageFullLabel=new QLabel(this);
}

void ImageFullDialog::initializeLayout() {
    verticalLayout=new QVBoxLayout();
    verticalLayout->addWidget(imageFullLabel);

    setLayout(verticalLayout);
}
