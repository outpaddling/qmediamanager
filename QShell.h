#include <QObject>

class QShell : public QObject
{
    Q_OBJECT

public slots:
    int     reformat(void);
    int     fm(void);
    void    unmount(void);
    int     minimize(void);
    void    setMount_point(char *mount_point);

private:
    char    *mount_point;

};

