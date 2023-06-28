#include <QPushButton>
#include <QVBoxLayout>
#include <QCloseEvent>
#include <unistd.h>         // fork()
#include <sys/wait.h>
#include <sys/stat.h>
#include <sysexits.h>
#include "QFormatMenu.hpp"
#include "misc.h"


QFormatMenu::QFormatMenu( char *new_mount_point, QWidget *parent  ) : QWidget(parent)

{
    QPushButton *exfat = new QPushButton("EXFAT (Windows and most Unix)", this),
		*ufs2 = new QPushButton("UFS2 (BSD)", this),
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


void    QFormatMenu::format(const char *fs_type)

{
    char    *device;
    char    message[POPUP_MSG_MAX + 1];
    char    temp_file[PATH_MAX + 1] = "/tmp/qmed.XXXXXXX";
    int     status;
    struct stat st;
    
    if ( mktemp(temp_file) == NULL )
    {
	popup("Error generating temp file name.");
	exit(EX_SOFTWARE);
    }

    if ( (device = get_device_name(mount_point)) != NULL )
    {
	if ( fork() == 0 )
	{
	    if ( umount(mount_point) == UMOUNT_OK )
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
	free(device);
    }
    else
    {
	snprintf(message, POPUP_MSG_MAX + 1,
		"%s is not mounted.  Cannot determine device name",
		mount_point);
	popup(message);
    }
}
