#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.setWindowTitle("Temperature Sensor Reading");
    w.setFixedSize(448,159);
    QIcon icon("C:/Users/josep/Downloads/GBTechWorld Icon.bmp");
    w.setWindowIcon(icon);
    w.show();

    return a.exec();
}
