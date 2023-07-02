#include <QApplication>
#include <stdlib.h>
#include <sysexits.h>

#include "QMediaMenu.hpp"

void    usage(char *argv[])

{
    // FIXME: Display error popup window
    fprintf(stderr, "Usage: %s /mount/point device fs-type\n", argv[0]);
    exit(EX_USAGE);
}


int     main(int argc, char *argv[])

{
    QApplication app(argc, argv);
    
    if ( argc != 4 )
	usage(argv);
    
    QMediaMenu *menu = new QMediaMenu(argv);
    menu->show();
    
    return app.exec();
}

