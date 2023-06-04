#include <QApplication>

#include "QMediaMenu.h"

int     main(int argc, char *argv[])

{
    QApplication app(argc, argv);
    
    QMediaMenu *menu = new QMediaMenu();
    
    //QT3 only app.setMainWidget( menu );
    menu->show();
    
    return app.exec();
}

