#ifndef _MENU_H
#define _MENU_H

#include <QWidget>

class QMediaMenu : public QWidget
{
    Q_OBJECT

public:
    //QMediaMenu( QWidget *parent = 0, int argc = 0, char *argv[] = NULL );
    QMediaMenu( int argc = 0, char *argv[] = NULL, QWidget *parent = 0 );

protected:
    void closeEvent (QCloseEvent *event);

private:
    char    *mount_point;

};

#endif

