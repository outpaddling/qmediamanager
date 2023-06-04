#include <QObject>

class QShell : public QObject
{
    Q_OBJECT

public slots:
    int     format(void);
    int     mount(void);
    int     unmount(void);

};

