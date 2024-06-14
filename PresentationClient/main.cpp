#include "presentationclient.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PresentationClient w;
    w.show();
    return a.exec();
}
