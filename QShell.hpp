#include <QObject>

class QShell : public QObject
{
    Q_OBJECT

public slots:
    int     fm(void);
    int     reformat(void);
    void    info(void);
    void    setMount_point(char *new_mount_point);
    void    setDevice(char *new_device);
    void    setFs_type(char *new_fs_type);
    int     image(void);
    void    unmount(void);

private:
    char    *mount_point;
    char    *device;
    char    *fs_type;
};

