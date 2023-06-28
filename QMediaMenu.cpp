#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QCloseEvent>
#include <sysexits.h>
#include "QShell.hpp"
#include "QMediaMenu.hpp"
#include "misc.h"

QMediaMenu::QMediaMenu( char *argv[], QWidget *parent  ) : QWidget(parent)

{
    char    unmount_text[UNMOUNT_TEXT_MAX + 1];

    mount_point = argv[1],
    snprintf(unmount_text, UNMOUNT_TEXT_MAX + 1, "Unmount %s", mount_point);
    
    QPushButton *info = new QPushButton("Filesystem info", this),
		*fm = new QPushButton("Open file manager", this),
		*reformat = new QPushButton("Reformat device", this),
		*image = new QPushButton("Copy image to device", this),
		*unmount = new QPushButton(unmount_text, this);
    QShell      *shell = new QShell;

    shell->setMount_point(mount_point);
    connect(info, SIGNAL(clicked()), shell, SLOT(info()));
    connect(fm, SIGNAL(clicked()), shell, SLOT(fm()));
    connect(reformat, SIGNAL(clicked()), shell, SLOT(reformat()));
    connect(image, SIGNAL(clicked()), shell, SLOT(image()));
    connect(unmount, SIGNAL(clicked()), shell, SLOT(unmount()));
    
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(info);
    layout->addWidget(fm);
    layout->addWidget(reformat);
    layout->addWidget(image);
    layout->addWidget(unmount);
    
    setLayout(layout);
}


/*
 *  Override closeEvent() so that user cannot close window before unmounting
 */

void QMediaMenu::closeEvent(QCloseEvent *event)

{
    popup("Use Unmount to unmount and close this window.");
    event->ignore();
}


