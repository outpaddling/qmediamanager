#include <QObject>

class QShell : public QObject
{
    Q_OBJECT

public slots:
    int     fm(void);
    int     reformat(void);
    void    info(void);
    void    setMount_point(char *mount_point);
    int     image(void);
    void    unmount(void);

private:
    char    *mount_point;
};

