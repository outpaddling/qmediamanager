#ifndef _MENU_H
#define _MENU_H

#include <QWidget>

class QMediaMenu : public QWidget
{
    Q_OBJECT

public:
    QMediaMenu( char *argv[] = NULL, QWidget *parent = 0 );

protected:
    void closeEvent (QCloseEvent *event);

private:
    char    *mount_point;
    char    *device;
    char    *fs_type;

};

#endif
