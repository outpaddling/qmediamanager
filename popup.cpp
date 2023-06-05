#include <QMessageBox>

void    popup(const char *message)

{
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.exec();
}
