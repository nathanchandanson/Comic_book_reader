#include <iostream>
#include <QApplication>

#include "Datatypes.hpp"
#include "Archive.hpp"
#include "MainWindow.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow widget;

    Comic imageVector;
    imageVector.push_back(QPixmap("../data/comics/testImage1.jpg"));
    imageVector.push_back(QPixmap("../data/comics/testImage2.jpg"));
    

    widget.show();
    return app.exec();
}