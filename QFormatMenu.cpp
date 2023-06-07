#include <QPushButton>
#include <QVBoxLayout>
#include <QCloseEvent>
#include <unistd.h>         // fork()
#include <sys/wait.h>
#include <sys/param.h>      // statfs()
#include <sys/mount.h>      // "
#include <sys/stat.h>
#include <sysexits.h>
#include "QFormatMenu.h"
#include "misc.h"


QFormatMenu::QFormatMenu( char *new_mount_point, QWidget *parent  ) : QWidget(parent)

{
    QPushButton *exfat = new QPushButton("EXFAT (Windows and most Unix)", this),
		*ufs2 = new QPushButton("UFS2 (FreeBSD)", this),
		*ext4 = new QPushButton("EXT4 (Linux)", this);

    mount_point = new_mount_point;
    
    connect(exfat, SIGNAL(clicked()), this, SLOT(exfat()));
    connect(ufs2, SIGNAL(clicked()), this, SLOT(ufs2()));
    connect(ext4, SIGNAL(clicked()), this, SLOT(ext4()));
    
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(exfat);
    layout->addWidget(ufs2);
    layout->addWidget(ext4);
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


void    QFormatMenu::ext4(void)

{
    format("ext4");
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


void    QFormatMenu::format(const char *fs_type)

{
    struct statfs   fs;
    char            message[POPUP_MSG_MAX + 1], *p;
    int             status;
    char            temp_file[PATH_MAX + 1] = "/tmp/qmed.XXXXXXX";
    char            cmd[CMD_MAX + 1];
    struct stat     st;
    
    if ( statfs(mount_point, &fs) == 0 )
    {
	device = fs.f_mntfromname;
	
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
	
	if ( mktemp(temp_file) == NULL )
	{
	    popup("Error generating temp file name.");
	    exit(EX_SOFTWARE);
	}
	
	if ( fork() == 0 )
	{
	    if ( umount() == UMOUNT_OK )
	    {
		execlp("urxvt", "urxvt", "-e", "auto-media-format",
			"--temp-file", temp_file,
			device, fs_type, NULL);
	
		// execlp() should not return
		popup("exec failed.");
	    }
	    else
		popup("Failed to unmount.");
	}
	else
	{
	    // FIXME: urxvt, xterm do not pass back exit status
	    wait(&status);
	    if ( stat(temp_file, &st) != 0 )
	    {
		popup("Format failed.");
		exit(EX_UNAVAILABLE);
	    }
	    else
	    {
		popup("Format complete.  Disconnect and reconnect\n"
		      "the device to mount the new filesystem.");
		unlink(temp_file);
		exit(EX_OK);
	    }
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
