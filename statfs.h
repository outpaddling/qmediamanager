// FIXME: Look for a more portable way to get FS info
#ifdef __FreeBSD__

// statvfs() does not expose all struct statfs members
#include <sys/param.h>  // statfs()
#include <sys/mount.h>  // "
typedef struct statfs statfs_t;
#define STATFS(path, st)  statfs(path, st)

#elif defined(__NetBSD__)

#include <sys/statvfs.h>
typedef struct statvfs statfs_t;
#define STATFS(path, st)  statvfs(path, st)

#endif
