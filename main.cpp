#include <QApplication>
#include "Client.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    app.setOrganizationName("Frolov Oleg SKB201");
    app.setApplicationName("TCP-Client");
    Client window;
    window.show();

    return app.exec();
}
