#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include "QShell.h"
#include "QMediaMenu.h"

QMediaMenu::QMediaMenu( QWidget *parent ) : QWidget(parent)

{
    QPushButton *mount = new QPushButton("Mount device", this),
		*unmount = new QPushButton("Unmount device", this),
		*format = new QPushButton("Format device", this),
		*quit = new QPushButton("Quit", this);
    QShell      *shell = new QShell;
    
    connect(mount, SIGNAL(clicked()), shell, SLOT(mount()));
    connect(unmount, SIGNAL(clicked()), shell, SLOT(unmount()));
    connect(format, SIGNAL(clicked()), shell, SLOT(format()));
    connect(quit, SIGNAL(clicked()), qApp, SLOT(quit()));
    
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(format);
    layout->addWidget(mount);
    layout->addWidget(unmount);
    layout->addWidget(quit);
    setLayout(layout);
}
