#include <QPushButton>
#include <QVBoxLayout>
#include <QCloseEvent>
#include <unistd.h>         // fork()
#include <sys/wait.h>
#include <sys/param.h>      // statfs()
#include <sys/mount.h>      // "
#include <sysexits.h>
#include "QFormatMenu.h"
#include "popup.h"

enum { OK, FAILED };

QFormatMenu::QFormatMenu( char *new_mount_point, QWidget *parent  ) : QWidget(parent)

{
    QPushButton *exfat = new QPushButton("EXFAT", this),
		*ufs2 = new QPushButton("UFS2", this);

    mount_point = new_mount_point;
    
    connect(exfat, SIGNAL(clicked()), this, SLOT(exfat()));
    connect(ufs2, SIGNAL(clicked()), this, SLOT(ufs2()));
    
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(exfat);
    layout->addWidget(ufs2);
    setLayout(layout);
}


void    QFormatMenu::exfat(void)

{
    format("exfat");
}


void    QFormatMenu::ufs2(void)

{
    format("ufs2");
}


// FIXME: Should this really be a member?

int     QFormatMenu::umount(void)

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
	return FAILED;
    }
    else
    {
	wait(&status);
	printf("umount status = %d\n", status);
	return OK;
	
	snprintf(message, POPUP_MSG_MAX + 1,
		 "qmediamanager: unmount failed: %s\n", strerror(errno));
	popup(message);
	return FAILED;
    }
}


void    QFormatMenu::format(const char *fs_type)

{
    struct statfs   fs;
    char            message[POPUP_MSG_MAX + 1], *p;
    int             status;
    
    if ( statfs(mount_point, &fs) == 0 )
    {
	// Remove partition such as "p1" or "s1"
	for (p = fs.f_mntfromname; !isdigit(*p) && *p != '\0'; ++p)
	    ;
	while ( isdigit(*p) )
	    ++p;
	*p = '\0';
	
	if ( fork() == 0 )
	{
	    if ( umount() == OK )
	    {
		execlp("urxvt", "urxvt", "-e", "auto-media-format",
			fs.f_mntfromname, fs_type, NULL);
	
		// execlp() should not return
		popup("exec failed.");
	    }
	    else
		popup("Failed to unmount.");
	}
	else
	{
	    wait(&status);
	    if ( status != 0 )
		popup("Format failed.");
	    else
		exit(EX_OK);
	}
    }
    else
    {
	snprintf(message, POPUP_MSG_MAX + 1,
		"%s is not mounted.  Cannot determine device name",
		mount_point);
	popup(message);
    }
}
