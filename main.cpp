#include <QApplication>

#include "QMediaMenu.h"

int     main(int argc, char *argv[])

{
    /* Debug
    int     c;
    FILE    *fp;
    
    fp = fopen("/home/bacon/argv", "a");
    for (c = 0; c < argc; ++c)
	fprintf(fp, "%s\n", argv[c]);
    fclose(fp);
    */
    
    QApplication app(argc, argv);
    
    QMediaMenu *menu = new QMediaMenu(argc, argv);
    menu->setAttribute(Qt::WA_QuitOnClose);
    
    // FIXME: Don't allow closing while device is mounted
    menu->show();
    
    return app.exec();
}

