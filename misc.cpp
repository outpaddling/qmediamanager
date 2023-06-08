#include <QMessageBox>
#include <unistd.h>
#include <pwd.h>

void    popup(const char *message)

{
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.exec();
}
