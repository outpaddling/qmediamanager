#include <QObject>

class QShell : public QObject
{
    Q_OBJECT

public slots:
    int     fm(void);
    void    unmount(void);
    int     reformat(void);
    void    info(void);
    void    setMount_point(char *mount_point);

private:
    char    *mount_point;
};

