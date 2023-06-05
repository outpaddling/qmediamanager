#include <iostream>
#include <QMessageBox>
#include <stdlib.h>
#include <sysexits.h>
#include <sys/stat.h>
#include <unistd.h>     // fork()
#include <sys/wait.h>
#include "QShell.h"

#define CMD_MAX 256
#define PREFIX  "/usr/local"

using namespace std;


void    QShell::setMount_point(char *new_mount_point)

{
    mount_point = new_mount_point;
}


int     QShell::fm(void)

{
    // FIXME: Find a more robust way to select a file manager
    // For now, check for likely add-ons first, then FMs that come
    // with desktop environments
    char    fms[][20] = { "pcmanfm-qt", "pcmanfm",
			  "caja", "dolphin", "lumina-fm", "konqueror",
			  "krusader", "nautilus", "nemo", "thunar", "" },
	    *fm,
	    path[PATH_MAX + 1];
    int     c, status;
    struct stat     st;
    
    if ( (fm = getenv("QMEDIA_FM")) == NULL )
    {
	for (c = 0; fms[c][0] != '\0'; ++c)
	{
	    snprintf(path, PATH_MAX + 1, "%s/bin/%s", PREFIX, fms[c]);
	    if ( stat(path, &st) == 0 )
	    {
		fm = fms[c];
		break;
	    }
	}
    }
    
    if ( fork() == 0 )
    {
	execlp(fm, fm, mount_point, NULL);
	
	// FIXME: popup message
	fprintf(stderr, "qmediamanager: exec failed: %s\n", strerror(errno));
	return EX_SOFTWARE;
    }
    else
    {
	wait(&status);
	// FIXME: popup message
	fprintf(stderr, "qmediamanager: File manager error: %s\n", strerror(errno));
	return status;
    }
    // FIXME: Use fork() and exec()
    // snprintf(cmd, CMD_MAX + 1, "%s %s", fm, mount_point);
    // return system(cmd);
}


void    QShell::unmount(void)

{
    int     status;
    
    // char    cmd[CMD_MAX + 1];
    
    // FIXME: Use fork() and exec()
    if ( fork() == 0 )
    {
	execlp("npmount", "npmount", "umount", mount_point, NULL);

	// FIXME: Popup message unmount failed
	fprintf(stderr, "qmediamanager: exec failed: %s\n", strerror(errno));
    }
    else
    {
	wait(&status);
	if ( status == 0 )
	    exit(EX_OK);
	
	// FIXME: Popup message unmount failed
	fprintf(stderr, "qmediamanager: unmount failed: %s\n", strerror(errno));
    }
}


int     QShell::reformat(void)

{
    QMessageBox msgBox;
    msgBox.setText("Reformat is not yet implemented.");
    msgBox.exec();
    
    return system("echo Reformat not yet implemented.");
}
