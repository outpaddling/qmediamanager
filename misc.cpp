#include <QMessageBox>
#include <unistd.h>
#include <pwd.h>

void    popup(const char *message)

{
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.exec();
}


/*
 *  Copied from libxtend to avoid an added dependency for 1 small function
 */

char   *get_home_dir(char *dir, size_t maxlen)

{
    int     user;
    struct passwd *pwentry;

    /* Determine who the user is */
    user = getuid();

    /* Get password file entry */
    if ((pwentry = getpwuid(user)) == NULL)
	return (NULL);
 
    strlcpy(dir, pwentry->pw_dir,maxlen);
    return dir;
}

