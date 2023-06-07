#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QCloseEvent>
#include <sysexits.h>
#include "QShell.h"
#include "QMediaMenu.h"
#include "misc.h"


QMediaMenu::QMediaMenu( char *argv[], QWidget *parent  ) : QWidget(parent)

{
    char    unmount_text[UNMOUNT_TEXT_MAX + 1];
    
    mount_point = argv[1],
    snprintf(unmount_text, UNMOUNT_TEXT_MAX + 1, "Unmount %s", mount_point);
    
    QPushButton *fm = new QPushButton("Open file manager", this),
		*unmount = new QPushButton(unmount_text, this),
		*reformat = new QPushButton("Reformat device", this);
    QShell      *shell = new QShell;

    shell->setMount_point(mount_point);
    connect(fm, SIGNAL(clicked()), shell, SLOT(fm()));
    connect(unmount, SIGNAL(clicked()), shell, SLOT(unmount()));
    connect(reformat, SIGNAL(clicked()), shell, SLOT(reformat()));
    
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(fm);
    layout->addWidget(unmount);
    layout->addWidget(reformat);
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


