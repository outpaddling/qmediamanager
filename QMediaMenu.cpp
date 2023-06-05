#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <sysexits.h>
#include "QShell.h"
#include "QMediaMenu.h"

#define UNMOUNT_TEXT_MAX    64

void    usage(char *argv[])

{
    // FIXME: Display error popup window
    fprintf(stderr, "Usage: %s /mount/point\n", argv[0]);
    exit(EX_USAGE);
}


QMediaMenu::QMediaMenu( int argc, char *argv[], QWidget *parent  ) : QWidget(parent)
// QMediaMenu::QMediaMenu( int argc, char *argv[] )

{
    char    unmount_text[UNMOUNT_TEXT_MAX + 1];
    
    if ( argc != 2 )
	usage(argv);
    
    mount_point = argv[1],
    snprintf(unmount_text, UNMOUNT_TEXT_MAX + 1, "Unmount %s", mount_point);
    
    QPushButton *fm = new QPushButton("Open file manager", this),
		*unmount = new QPushButton(unmount_text, this),
		*reformat = new QPushButton("Reformat device", this),
		*minimize = new QPushButton("Minimize this popup", this);
    QShell      *shell = new QShell;

    shell->setMount_point(mount_point);
    connect(fm, SIGNAL(clicked()), shell, SLOT(fm()));
    connect(unmount, SIGNAL(clicked()), shell, SLOT(unmount()));
    connect(reformat, SIGNAL(clicked()), shell, SLOT(reformat()));
    connect(minimize, SIGNAL(clicked()), shell, SLOT(minimize()));
    
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(fm);
    layout->addWidget(unmount);
    layout->addWidget(reformat);
    layout->addWidget(minimize);
    setLayout(layout);
}
