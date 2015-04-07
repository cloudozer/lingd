/* config/config.h.  Generated from config.h.in by configure.  */
/* config/config.h.in.  Generated from configure.ac by autoheader.  */

/* Define if building universal (internal helper macro) */
/* #undef AC_APPLE_UNIVERSAL_BUILD */

/* Define to 1 if authenticating via AUTH_METHOD_GETPEEREID */
/* #undef AUTH_METHOD_GETPEEREID */

/* Define to 1 if authenticating via AUTH_METHOD_GETPEERUCRED */
/* #undef AUTH_METHOD_GETPEERUCRED */

/* Define to 1 if authenticating via AUTH_METHOD_LOCAL_PEERCRED */
/* #undef AUTH_METHOD_LOCAL_PEERCRED */

/* Define to 1 if authenticating via AUTH_METHOD_RECVFD_MKFIFO */
/* #undef AUTH_METHOD_RECVFD_MKFIFO */

/* Define to 1 if authenticating via AUTH_METHOD_RECVFD_MKNOD */
/* #undef AUTH_METHOD_RECVFD_MKNOD */

/* Define to 1 if authenticating via AUTH_METHOD_SO_PEERCRED */
#define AUTH_METHOD_SO_PEERCRED 1

/* Define to 1 if you have the <bzlib.h> header file. */
//#define HAVE_BZLIB_H 1

/* Define to 1 if the system has the type `CRYPTO_dynlock'. */
#define HAVE_CRYPTO_DYNLOCK 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the `ERR_error_string_n' function. */
#define HAVE_ERR_ERROR_STRING_N 1

/* Define to 1 if you have the `EVP_aes_128_cbc' function. */
#define HAVE_EVP_AES_128_CBC 1

/* Define to 1 if you have the `EVP_aes_256_cbc' function. */
#define HAVE_EVP_AES_256_CBC 1

/* Define to 1 if you have the `EVP_CipherInit_ex' function. */
#define HAVE_EVP_CIPHERINIT_EX 1

/* Define to 1 if you have the `EVP_DigestInit_ex' function. */
#define HAVE_EVP_DIGESTINIT_EX 1

/* Define to 1 if you have the `EVP_sha256' function. */
#define HAVE_EVP_SHA256 1

/* Define to 1 if you have the `EVP_sha512' function. */
#define HAVE_EVP_SHA512 1

/* Define to 1 if file descriptors can be passed over a fifo. */
/* #undef HAVE_FIFO_RECVFD */

/* Define to 1 if you have the `getgrent' function. */
#define HAVE_GETGRENT 1

/* Define to 1 if you have the `getgrent_r' function from AIX. */
/* #undef HAVE_GETGRENT_R_AIX */

/* Define to 1 if you have the `getgrent_r' function from GNU. */
#define HAVE_GETGRENT_R_GNU 1

/* Define to 1 if you have the `getgrent_r' function from SunOS. */
/* #undef HAVE_GETGRENT_R_SUN */

/* Define to 1 if you have the `getpeereid' function. */
/* #undef HAVE_GETPEEREID */

/* Define to 1 if you have the `getpeerucred' function. */
/* #undef HAVE_GETPEERUCRED */

/* Define to 1 if you have the `getpwnam' function. */
#define HAVE_GETPWNAM 1

/* Define to 1 if you have the `getpwnam_r' function from AIX. */
/* #undef HAVE_GETPWNAM_R_AIX */

/* Define to 1 if you have the `getpwnam_r' function from POSIX. */
#define HAVE_GETPWNAM_R_POSIX 1

/* Define to 1 if you have the `getpwnam_r' function from SunOS. */
/* #undef HAVE_GETPWNAM_R_SUN */

/* Define to 1 if you have the `HMAC_Init_ex' function. */
#define HAVE_HMAC_INIT_EX 1

/* Define to 1 if you have the `inet_ntop' function. */
#define HAVE_INET_NTOP 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `bz2' library (-lbz2). */
#define HAVE_LIBBZ2 1

/* Define to 1 if you want to use the Libgcrypt cryptographic library. */
/* #undef HAVE_LIBGCRYPT */

/* Define to 1 if you have the `z' library (-lz). */
#define HAVE_LIBZ 1

/* Define to 1 if you have the `localtime_r' function. */
#define HAVE_LOCALTIME_R 1

/* Define to 1 if you have the LOCAL_PEERCRED socket option. */
/* #undef HAVE_LOCAL_PEERCRED */

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `mlockall' function. */
#define HAVE_MLOCKALL 1

/* Define to 1 if you want to use the OpenSSL cryptographic library. */
#define HAVE_OPENSSL 1

/* Define to 1 if the system has the type `socklen_t'. */
#define HAVE_SOCKLEN_T 1

/* Define to 1 if you have the SO_PEERCRED socket option. */
#define HAVE_SO_PEERCRED 1

/* Define to 1 if you have the <standards.h> header file. */
/* #undef HAVE_STANDARDS_H */

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strlcat' function. */
/* #undef HAVE_STRLCAT */

/* Define to 1 if you have the `strlcpy' function. */
/* #undef HAVE_STRLCPY */

/* Define to 1 if the system has the type `struct strrecvfd'. */
#define HAVE_STRUCT_STRRECVFD 1

/* Define to 1 if the system has the type `struct ucred'. */
#define HAVE_STRUCT_UCRED 1

/* Define to 1 if the system has the type `struct xucred'. */
/* #undef HAVE_STRUCT_XUCRED */

/* Define to 1 if you have the `sysconf' function. */
#define HAVE_SYSCONF 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <ucred.h> header file. */
/* #undef HAVE_UCRED_H */

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the <zlib.h> header file. */
//#define HAVE_ZLIB_H 1

/* Define to 1 if you have the file `/dev/spx'. */
/* #undef HAVE__DEV_SPX */

/* Define the canonical host type. */
#define HOST "x86_64-unknown-linux-gnu"

/* Define the canonical host CPU type. */
#define HOST_CPU "x86_64"

/* Define the canonical host OS type. */
#define HOST_OS "linux-gnu"

/* Define the canonical host vendor type. */
#define HOST_VENDOR "unknown"

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#define LT_OBJDIR ".libs/"

/* Define the project alias string (name-ver or name-ver-rel). */
#define META_ALIAS "munge-0.5.11"

/* Define the project author. */
#define META_AUTHOR "Chris Dunlap <cdunlap@llnl.gov>"

/* Define the project release date. */
#define META_DATE "2013-08-27"

/* Define the libtool library 'age' version information. */
#define META_LT_AGE "0"

/* Define the libtool library 'current' version information. */
#define META_LT_CURRENT "2"

/* Define the libtool library 'revision' version information. */
#define META_LT_REVISION "0"

/* Define the project name. */
#define META_NAME "munge"

/* Define the project release. */
#define META_RELEASE "1"

/* Define the project version. */
#define META_VERSION "0.5.11"

/* Define to 1 if you are building a production release. */
/* #undef NDEBUG */

/* Define to 1 if your C compiler doesn't accept -c and -o together. */
/* #undef NO_MINUS_C_MINUS_O */

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME ""

/* Define to the full name and version of this package. */
#define PACKAGE_STRING ""

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME ""

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION ""

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Enable extensions on AIX 3, Interix.  */
#ifndef _ALL_SOURCE
# define _ALL_SOURCE 1
#endif
/* Enable GNU extensions on systems that have them.  */
#ifndef _GNU_SOURCE
# define _GNU_SOURCE 1
#endif
/* Enable threading extensions on Solaris.  */
#ifndef _POSIX_PTHREAD_SEMANTICS
# define _POSIX_PTHREAD_SEMANTICS 1
#endif
/* Enable extensions on HP NonStop.  */
#ifndef _TANDEM_SOURCE
# define _TANDEM_SOURCE 1
#endif
/* Enable general extensions on Solaris.  */
#ifndef __EXTENSIONS__
# define __EXTENSIONS__ 1
#endif


/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#if defined AC_APPLE_UNIVERSAL_BUILD
# if defined __BIG_ENDIAN__
#  define WORDS_BIGENDIAN 1
# endif
#else
# ifndef WORDS_BIGENDIAN
/* #  undef WORDS_BIGENDIAN */
# endif
#endif

/* Expansion of the "bindir" installation directory. */
#define X_BINDIR "/usr/bin"

/* Expansion of the "datadir" installation directory. */
#define X_DATADIR "/usr/share"

/* Expansion of the "exec_prefix" installation directory. */
#define X_EXEC_PREFIX "/usr"

/* Expansion of the "includedir" installation directory. */
#define X_INCLUDEDIR "/usr/include"

/* Expansion of the "infodir" installation directory. */
#define X_INFODIR "/usr/share/info"

/* Expansion of the "libdir" installation directory. */
#define X_LIBDIR "/usr/lib"

/* Expansion of the "libexecdir" installation directory. */
#define X_LIBEXECDIR "/usr/libexec"

/* Expansion of the "localstatedir" installation directory. */
#define X_LOCALSTATEDIR "/var"

/* Expansion of the "mandir" installation directory. */
#define X_MANDIR "/usr/share/man"

/* Expansion of the "oldincludedir" installation directory. */
#define X_OLDINCLUDEDIR "/usr/include"

/* Expansion of the "prefix" installation directory. */
#define X_PREFIX "/usr"

/* Expansion of the "sbindir" installation directory. */
#define X_SBINDIR "/usr/bin"

/* Expansion of the "sharedstatedir" installation directory. */
#define X_SHAREDSTATEDIR "/usr/com"

/* Expansion of the "sysconfdir" installation directory. */
#define X_SYSCONFDIR "/etc"

/* Define to 1 if you are building on Darwin (Mac OS X). */
/* #undef _APPLE_C_SOURCE */

/* Define to 1 if on MINIX. */
/* #undef _MINIX */

/* Define to 2 if the system does not provide POSIX.1 features except with
   this defined. */
/* #undef _POSIX_1_SOURCE */

/* Define to 1 if you need to in order for `stat' and other things to work. */
/* #undef _POSIX_SOURCE */

/* Define to 1 if you plan to link against multithreaded code. */
#define _REENTRANT 1

/* Define to 1 if you plan to link against multithreaded code. */
#define _THREAD_SAFE 1

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Define to `int' if <sys/types.h> doesn't define. */
/* #undef gid_t */

/* Define to `int' if <sys/types.h> doesn't define. */
/* #undef uid_t */
