#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Create and show the main window
    MainWindow window;
    window.show();

    return app.exec();
}