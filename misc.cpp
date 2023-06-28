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


char    *get_device_name(const char *mount_point)

{
    statfs_t    fs;
    char        *device, *p;
    char        cmd[CMD_MAX + 1];
    
    if ( STATFS(mount_point, &fs) == 0 )
    {
	device = strdup(fs.f_mntfromname);
	
	// FIXME: This is a fragile hack that depends on the syntax of the
	// lklfuse command.  Is there a better way to get the device name
	// behind lklfuse?
	if ( strcmp(device, "lklfuse") == 0 )
	{
	    FILE    *fp;
	    
	    snprintf(cmd, CMD_MAX + 1, 
		    "ps axww | grep 'lklfuse.*%s' | grep -v grep | awk '{ print $(NF-1) }'",
		    mount_point);
	    fp = popen(cmd, "r");
	    if ( fp == NULL )
	    {
		popup("Cannot determine lklfuse device name.");
		exit(EX_SOFTWARE);
	    }
	    fgets(device, 20, fp);
	    device[strlen(device)-1] = '\0';
	    pclose(fp);
	}
	
	// Remove partition such as "p1" or "s1"
	for (p = device; !isdigit(*p) && *p != '\0'; ++p)
	    ;
	while ( isdigit(*p) )
	    ++p;
	*p = '\0';

	return device;
    }
    else
	return NULL;
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
