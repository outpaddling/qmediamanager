#include <QMessageBox>
#include <unistd.h>
#include <pwd.h>
#include <sysexits.h>
#include <sys/wait.h>
#include "statfs.h"
#include "misc.h"

void    popup(const char *message)

{
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.exec();
}


char    *parent_device_name(const char *partition)

{
    char        *device, *p;

    device = strdup(partition);

    if ( device != NULL )
    {
	// Strip off everything after the first number, e.g.
	// sd0a -> sd0, da0p1 -> da0
	for (p = device; !isdigit(*p) && *p != '\0'; ++p)
	    ;
	while ( isdigit(*p) )
	    ++p;
	*p = '\0';
    }
    
    return device;
}


int     umount(const char *mount_point)

{
    int     status;
    char    message[POPUP_MSG_MAX + 1];
    
    if ( fork() == 0 )
    {
	execlp("npmount", "npmount", "umount", mount_point, NULL);
	
	// execlp() should not return
	snprintf(message, POPUP_MSG_MAX + 1,
		 "qmediamanager: exec failed: %s\n", strerror(errno));
	popup(message);
	return UMOUNT_FAILED;
    }
    else
    {
	wait(&status);
	// popup() causes hang here ??
	//snprintf(message, POPUP_MSG_MAX + 1,
	//         "umount status = %d\n", status);
	//popup(message);
	if ( status == 0 )
	    return UMOUNT_OK;
	
	snprintf(message, POPUP_MSG_MAX + 1,
		 "qmediamanager: unmount failed: %s\n", strerror(errno));
	popup(message);
	return UMOUNT_FAILED;
    }
}
