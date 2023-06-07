#ifndef _FORMATMENU_H
#define _FORMATMENU_H

#include <QWidget>

class QFormatMenu : public QWidget
{
    Q_OBJECT

public:
    QFormatMenu( char *new_mount_point = 0, QWidget *parent = 0 );

private slots:
    void    exfat(void);
    void    ufs2(void);

private:
    char    *mount_point;
    char    *device;
    int     umount(void);
    void    format(const char *fs_type);

};

#endif
