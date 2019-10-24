/* Target value mapping utilities needed by the simulator and gdb.  */
/* This file is machine generated by gentmap.c.  */

#include "config.h"
#include <errno.h>
#include <fcntl.h>
#include "ansidecl.h"
#include "gdb/callback.h"
#include "targ-vals.h"

/* syscall mapping table */
CB_TARGET_DEFS_MAP cb_init_syscall_map[] = {
  { "close", CB_SYS_close, TARGET_SYS_close},
  { "lseek", CB_SYS_lseek, TARGET_SYS_lseek},
  { "read", CB_SYS_read, TARGET_SYS_read},
  { "write", CB_SYS_write, TARGET_SYS_write},
  { "fstat", CB_SYS_fstat, TARGET_SYS_fstat},
  { "exit", CB_SYS_exit, TARGET_SYS_exit},
  { "open", CB_SYS_open, TARGET_SYS_open},
  { "unlink", CB_SYS_unlink, TARGET_SYS_unlink},
  { "stat", CB_SYS_stat, TARGET_SYS_stat},
  { 0, -1, -1 }
};

/* errno mapping table */
CB_TARGET_DEFS_MAP cb_init_errno_map[] = {
#ifdef E2BIG
  { "E2BIG", E2BIG, TARGET_E2BIG },
#endif
#ifdef EACCES
  { "EACCES", EACCES, TARGET_EACCES },
#endif
#ifdef EADDRINUSE
  { "EADDRINUSE", EADDRINUSE, TARGET_EADDRINUSE },
#endif
#ifdef EADDRNOTAVAIL
  { "EADDRNOTAVAIL", EADDRNOTAVAIL, TARGET_EADDRNOTAVAIL },
#endif
#ifdef EAFNOSUPPORT
  { "EAFNOSUPPORT", EAFNOSUPPORT, TARGET_EAFNOSUPPORT },
#endif
#ifdef EAGAIN
  { "EAGAIN", EAGAIN, TARGET_EAGAIN },
#endif
#ifdef EALREADY
  { "EALREADY", EALREADY, TARGET_EALREADY },
#endif
#ifdef EBADF
  { "EBADF", EBADF, TARGET_EBADF },
#endif
#ifdef EBADMSG
  { "EBADMSG", EBADMSG, TARGET_EBADMSG },
#endif
#ifdef EBUSY
  { "EBUSY", EBUSY, TARGET_EBUSY },
#endif
#ifdef ECANCELED
  { "ECANCELED", ECANCELED, TARGET_ECANCELED },
#endif
#ifdef ECHILD
  { "ECHILD", ECHILD, TARGET_ECHILD },
#endif
#ifdef ECONNABORTED
  { "ECONNABORTED", ECONNABORTED, TARGET_ECONNABORTED },
#endif
#ifdef ECONNREFUSED
  { "ECONNREFUSED", ECONNREFUSED, TARGET_ECONNREFUSED },
#endif
#ifdef ECONNRESET
  { "ECONNRESET", ECONNRESET, TARGET_ECONNRESET },
#endif
#ifdef EDEADLK
  { "EDEADLK", EDEADLK, TARGET_EDEADLK },
#endif
#ifdef EDESTADDRREQ
  { "EDESTADDRREQ", EDESTADDRREQ, TARGET_EDESTADDRREQ },
#endif
#ifdef EDOM
  { "EDOM", EDOM, TARGET_EDOM },
#endif
#ifdef EDQUOT
  { "EDQUOT", EDQUOT, TARGET_EDQUOT },
#endif
#ifdef EEXIST
  { "EEXIST", EEXIST, TARGET_EEXIST },
#endif
#ifdef EFAULT
  { "EFAULT", EFAULT, TARGET_EFAULT },
#endif
#ifdef EFBIG
  { "EFBIG", EFBIG, TARGET_EFBIG },
#endif
#ifdef EFTYPE
  { "EFTYPE", EFTYPE, TARGET_EFTYPE },
#endif
#ifdef EHOSTDOWN
  { "EHOSTDOWN", EHOSTDOWN, TARGET_EHOSTDOWN },
#endif
#ifdef EHOSTUNREACH
  { "EHOSTUNREACH", EHOSTUNREACH, TARGET_EHOSTUNREACH },
#endif
#ifdef EIDRM
  { "EIDRM", EIDRM, TARGET_EIDRM },
#endif
#ifdef EILSEQ
  { "EILSEQ", EILSEQ, TARGET_EILSEQ },
#endif
#ifdef EINPROGRESS
  { "EINPROGRESS", EINPROGRESS, TARGET_EINPROGRESS },
#endif
#ifdef EINTR
  { "EINTR", EINTR, TARGET_EINTR },
#endif
#ifdef EINVAL
  { "EINVAL", EINVAL, TARGET_EINVAL },
#endif
#ifdef EIO
  { "EIO", EIO, TARGET_EIO },
#endif
#ifdef EISCONN
  { "EISCONN", EISCONN, TARGET_EISCONN },
#endif
#ifdef EISDIR
  { "EISDIR", EISDIR, TARGET_EISDIR },
#endif
#ifdef ELOOP
  { "ELOOP", ELOOP, TARGET_ELOOP },
#endif
#ifdef EMFILE
  { "EMFILE", EMFILE, TARGET_EMFILE },
#endif
#ifdef EMLINK
  { "EMLINK", EMLINK, TARGET_EMLINK },
#endif
#ifdef EMSGSIZE
  { "EMSGSIZE", EMSGSIZE, TARGET_EMSGSIZE },
#endif
#ifdef EMULTIHOP
  { "EMULTIHOP", EMULTIHOP, TARGET_EMULTIHOP },
#endif
#ifdef ENAMETOOLONG
  { "ENAMETOOLONG", ENAMETOOLONG, TARGET_ENAMETOOLONG },
#endif
#ifdef ENETDOWN
  { "ENETDOWN", ENETDOWN, TARGET_ENETDOWN },
#endif
#ifdef ENETRESET
  { "ENETRESET", ENETRESET, TARGET_ENETRESET },
#endif
#ifdef ENETUNREACH
  { "ENETUNREACH", ENETUNREACH, TARGET_ENETUNREACH },
#endif
#ifdef ENFILE
  { "ENFILE", ENFILE, TARGET_ENFILE },
#endif
#ifdef ENOBUFS
  { "ENOBUFS", ENOBUFS, TARGET_ENOBUFS },
#endif
#ifdef ENODATA
  { "ENODATA", ENODATA, TARGET_ENODATA },
#endif
#ifdef ENODEV
  { "ENODEV", ENODEV, TARGET_ENODEV },
#endif
#ifdef ENOENT
  { "ENOENT", ENOENT, TARGET_ENOENT },
#endif
#ifdef ENOEXEC
  { "ENOEXEC", ENOEXEC, TARGET_ENOEXEC },
#endif
#ifdef ENOLCK
  { "ENOLCK", ENOLCK, TARGET_ENOLCK },
#endif
#ifdef ENOLINK
  { "ENOLINK", ENOLINK, TARGET_ENOLINK },
#endif
#ifdef ENOMEM
  { "ENOMEM", ENOMEM, TARGET_ENOMEM },
#endif
#ifdef ENOMSG
  { "ENOMSG", ENOMSG, TARGET_ENOMSG },
#endif
#ifdef ENOPROTOOPT
  { "ENOPROTOOPT", ENOPROTOOPT, TARGET_ENOPROTOOPT },
#endif
#ifdef ENOSPC
  { "ENOSPC", ENOSPC, TARGET_ENOSPC },
#endif
#ifdef ENOSR
  { "ENOSR", ENOSR, TARGET_ENOSR },
#endif
#ifdef ENOSTR
  { "ENOSTR", ENOSTR, TARGET_ENOSTR },
#endif
#ifdef ENOSYS
  { "ENOSYS", ENOSYS, TARGET_ENOSYS },
#endif
#ifdef ENOTCONN
  { "ENOTCONN", ENOTCONN, TARGET_ENOTCONN },
#endif
#ifdef ENOTDIR
  { "ENOTDIR", ENOTDIR, TARGET_ENOTDIR },
#endif
#ifdef ENOTEMPTY
  { "ENOTEMPTY", ENOTEMPTY, TARGET_ENOTEMPTY },
#endif
#ifdef ENOTRECOVERABLE
  { "ENOTRECOVERABLE", ENOTRECOVERABLE, TARGET_ENOTRECOVERABLE },
#endif
#ifdef ENOTSOCK
  { "ENOTSOCK", ENOTSOCK, TARGET_ENOTSOCK },
#endif
#ifdef ENOTSUP
  { "ENOTSUP", ENOTSUP, TARGET_ENOTSUP },
#endif
#ifdef ENOTTY
  { "ENOTTY", ENOTTY, TARGET_ENOTTY },
#endif
#ifdef ENXIO
  { "ENXIO", ENXIO, TARGET_ENXIO },
#endif
#ifdef EOPNOTSUPP
  { "EOPNOTSUPP", EOPNOTSUPP, TARGET_EOPNOTSUPP },
#endif
#ifdef EOVERFLOW
  { "EOVERFLOW", EOVERFLOW, TARGET_EOVERFLOW },
#endif
#ifdef EOWNERDEAD
  { "EOWNERDEAD", EOWNERDEAD, TARGET_EOWNERDEAD },
#endif
#ifdef EPERM
  { "EPERM", EPERM, TARGET_EPERM },
#endif
#ifdef EPFNOSUPPORT
  { "EPFNOSUPPORT", EPFNOSUPPORT, TARGET_EPFNOSUPPORT },
#endif
#ifdef EPIPE
  { "EPIPE", EPIPE, TARGET_EPIPE },
#endif
#ifdef EPROTO
  { "EPROTO", EPROTO, TARGET_EPROTO },
#endif
#ifdef EPROTONOSUPPORT
  { "EPROTONOSUPPORT", EPROTONOSUPPORT, TARGET_EPROTONOSUPPORT },
#endif
#ifdef EPROTOTYPE
  { "EPROTOTYPE", EPROTOTYPE, TARGET_EPROTOTYPE },
#endif
#ifdef ERANGE
  { "ERANGE", ERANGE, TARGET_ERANGE },
#endif
#ifdef EROFS
  { "EROFS", EROFS, TARGET_EROFS },
#endif
#ifdef ESPIPE
  { "ESPIPE", ESPIPE, TARGET_ESPIPE },
#endif
#ifdef ESRCH
  { "ESRCH", ESRCH, TARGET_ESRCH },
#endif
#ifdef ESTALE
  { "ESTALE", ESTALE, TARGET_ESTALE },
#endif
#ifdef ETIME
  { "ETIME", ETIME, TARGET_ETIME },
#endif
#ifdef ETIMEDOUT
  { "ETIMEDOUT", ETIMEDOUT, TARGET_ETIMEDOUT },
#endif
#ifdef ETOOMANYREFS
  { "ETOOMANYREFS", ETOOMANYREFS, TARGET_ETOOMANYREFS },
#endif
#ifdef ETXTBSY
  { "ETXTBSY", ETXTBSY, TARGET_ETXTBSY },
#endif
#ifdef EWOULDBLOCK
  { "EWOULDBLOCK", EWOULDBLOCK, TARGET_EWOULDBLOCK },
#endif
#ifdef EXDEV
  { "EXDEV", EXDEV, TARGET_EXDEV },
#endif
  { 0, 0, 0 }
};

/* open flags mapping table */
CB_TARGET_DEFS_MAP cb_init_open_map[] = {
#ifdef O_ACCMODE
  { "O_ACCMODE", O_ACCMODE, TARGET_O_ACCMODE },
#endif
#ifdef O_APPEND
  { "O_APPEND", O_APPEND, TARGET_O_APPEND },
#endif
#ifdef O_CREAT
  { "O_CREAT", O_CREAT, TARGET_O_CREAT },
#endif
#ifdef O_EXCL
  { "O_EXCL", O_EXCL, TARGET_O_EXCL },
#endif
#ifdef O_NOCTTY
  { "O_NOCTTY", O_NOCTTY, TARGET_O_NOCTTY },
#endif
#ifdef O_NONBLOCK
  { "O_NONBLOCK", O_NONBLOCK, TARGET_O_NONBLOCK },
#endif
#ifdef O_RDONLY
  { "O_RDONLY", O_RDONLY, TARGET_O_RDONLY },
#endif
#ifdef O_RDWR
  { "O_RDWR", O_RDWR, TARGET_O_RDWR },
#endif
#ifdef O_SYNC
  { "O_SYNC", O_SYNC, TARGET_O_SYNC },
#endif
#ifdef O_TRUNC
  { "O_TRUNC", O_TRUNC, TARGET_O_TRUNC },
#endif
#ifdef O_WRONLY
  { "O_WRONLY", O_WRONLY, TARGET_O_WRONLY },
#endif
  { 0, -1, -1 }
};

