#ifndef IMAGEFULLDIALOG_H
#define IMAGEFULLDIALOG_H

#include <QDialog>
#include <QString>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>

class ImageFullDialog : public QDialog {
    Q_OBJECT

public:
    ImageFullDialog(QWidget *parent=nullptr, QPixmap *image=nullptr);
    ~ImageFullDialog();
    QLabel *imageFullLabel;
    QVBoxLayout *verticalLayout;

private:
    void initializeWidgets();
    void initializeLayout();

    QPixmap *image;
};
#endif // IMAGEFULLDIALOG_H
