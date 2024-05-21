#include <QApplication>
#include "client.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Client client;
    client.connectToServer("127.0.0.1", 12345);
    client.show();
    return a.exec();
}
