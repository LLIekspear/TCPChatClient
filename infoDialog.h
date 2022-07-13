#ifndef INFODIALOG_H
#define INFODIALOG_H

#include <QDialog>
#include <QString>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>

class InfoDialog : public QDialog {
    Q_OBJECT

public:
    InfoDialog(QWidget *parent=nullptr);
    ~InfoDialog();

public slots:
    void on_closeButton_clicked();

private:
    void initializeWidgets();
    void initializeLayout();

    QLabel *infoLabel;
    QLabel *dateBuildLabel;
    QLabel *qtversionbuildLabel;
    QLabel *qtversionstartedLabel;

    QPushButton *closeButton;

    QHBoxLayout *findHorizontalLayout;
    QHBoxLayout *dateBuildHorizontalLayout;
    QHBoxLayout *qtversionbuildHorizontalLayout;
    QHBoxLayout *qtversionstartedHorizontalLayout;
    QVBoxLayout *verticalLayout;
};

#endif // INFODIALOG_H
