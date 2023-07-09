
#define POPUP_MSG_MAX       1024
#define UNMOUNT_TEXT_MAX    64
#define FORMAT_TEXT_MAX     64
#define IMAGE_TEXT_MAX      64
#define CMD_MAX             1024
#define GIB                 (1024.0 * 1024.0 * 1024.0)
#define PREFIX              "/usr/local"
#define RECONNECT_MSG       "  You may need to disconnect and\n" \
			    "reconnect the device to mount all new filesystems."

enum { UMOUNT_OK, UMOUNT_FAILED };

void    popup(const char *message);
char    *parent_device_name(const char *partition);
int     umount(const char *mount_point);
