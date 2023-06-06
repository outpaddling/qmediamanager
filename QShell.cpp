#include <iostream>
#include <stdlib.h>
#include <sysexits.h>
#include <sys/stat.h>
#include <unistd.h>     // fork()
#include <sys/wait.h>
#include "QShell.h"
#include "QFormatMenu.h"
#include "popup.h"

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
	    path[PATH_MAX + 1],
	    message[POPUP_MSG_MAX + 1];
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
	
	snprintf(message, POPUP_MSG_MAX + 1,
		 "qmediamanager: exec failed: %s\n", strerror(errno));
	popup(message);
	return EX_SOFTWARE;
    }
    else
    {
	wait(&status);
	if ( status != 0 )
	{
	    snprintf(message, POPUP_MSG_MAX + 1,
		     "qmediamanager: File manager error: %s\n", strerror(errno));
	    popup(message);
	}
	return status;
    }
}


void    QShell::unmount(void)

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
    }
    else
    {
	wait(&status);
	printf("umount status = %d\n", status);
	if ( status == 0 )
	    exit(EX_OK);
	
	snprintf(message, POPUP_MSG_MAX + 1,
		 "qmediamanager: unmount failed: %s\n", strerror(errno));
	popup(message);
    }
}


int     QShell::reformat(void)

{
    // popup("Reformat is not yet implemented.");

    QFormatMenu *menu = new QFormatMenu(mount_point);
    menu->show();

    return EX_OK;
}
