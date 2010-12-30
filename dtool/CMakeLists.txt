include(CheckIncludeFileCXX)
include(CheckFunctionExists)
include(CheckTypeSize)

#/* Define if your processor stores words with the most significant
#   byte first (like Motorola and SPARC, unlike Intel and VAX).  */
#$[cdefine WORDS_BIGENDIAN]

#/* Define if the C++ compiler uses namespaces.  */
set(HAVE_NAMESPACE 1)

#/* Define if fstream::open() accepts a third parameter for umask. */
#$[cdefine HAVE_OPEN_MASK]

check_type_size(wchar_t HAVE_WCHAR_T)
set(HAVE_WSTRING 1)

#/* Define if the C++ compiler supports the typename keyword.  */
set(HAVE_TYPENAME 1)

#/* Define if we can trust the compiler not to insert extra bytes in
#   structs between base structs and derived structs. */
#$[cdefine SIMPLE_STRUCT_POINTERS]

#/* Define if we have Dinkumware STL installed.  */
#$[cdefine HAVE_DINKUM]

#/* Define if we have STL hash_map etc. available  */
#$[cdefine HAVE_STL_HASH]

check_function_exists(gettimeofday HAVE_GETTIMEOFDAY)

check_function_exists(getopt HAVE_GETOPT)
check_function_exists(getopt_long_only HAVE_GETOPT_LONG_ONLY)
check_include_file_cxx(getopt.h PHAVE_GETOPT_H)

#/* Define if you have ioctl(TIOCGWINSZ) to determine terminal width. */
#$[cdefine IOCTL_TERMINAL_WIDTH]

set(HAVE_STREAMSIZE 1)

#/* Do the system headers define key ios typedefs like ios::openmode
#   and ios::fmtflags? */
set(HAVE_IOS_TYPEDEFS 1)

#/* Define if the C++ iostream library defines ios::binary.  */
set(HAVE_IOS_BINARY 1)

#/* Can we safely call getenv() at static init time? */
set(STATIC_INIT_GETENV 1)

set(DEFAULT_PATHSEP ":")

#$[cdefine HAVE_PROC_SELF_EXE]
#$[cdefine HAVE_PROC_SELF_MAPS]
#$[cdefine HAVE_PROC_SELF_ENVIRON]
#$[cdefine HAVE_PROC_SELF_CMDLINE]
#$[cdefine HAVE_PROC_CURPROC_FILE]
#$[cdefine HAVE_PROC_CURPROC_MAP]
#$[cdefine HAVE_PROC_CURPROC_CMDLINE]

#/* Do we have a global pair of argc/argv variables that we can read at
#   static init time?  Should we prototype them?  What are they called? */
#$[cdefine HAVE_GLOBAL_ARGV]
#$[cdefine PROTOTYPE_GLOBAL_ARGV]
#$[cdefine GLOBAL_ARGV]
#$[cdefine GLOBAL_ARGC]

check_include_file_cxx(io.h PHAVE_IO_H)
check_include_file_cxx(iostream PHAVE_IOSTREAM)
check_include_file_cxx(malloc.h PHAVE_MALLOC_H)
check_include_file_cxx(sys/malloc.h PHAVE_SYS_MALLOC_H)
check_include_file_cxx(alloca.h PHAVE_ALLOCA_H)
check_include_file_cxx(locale.h PHAVE_LOCALE_H)
check_include_file_cxx(string.h PHAVE_STRING_H)
check_include_file_cxx(stdlib.h PHAVE_STDLIB_H)
check_include_file_cxx(limits.h PHAVE_LIMITS_H)
check_include_file_cxx(minmax.h PHAVE_MINMAX_H)
check_include_file_cxx(sstream PHAVE_SSTREAM)
check_include_file_cxx(new PHAVE_NEW)
check_include_file_cxx(sys/types.h PHAVE_SYS_TYPES_H)
check_include_file_cxx(sys/time.h PHAVE_SYS_TIME_H)
check_include_file_cxx(unistd.h PHAVE_UNISTD_H)
check_include_file_cxx(utime.h PHAVE_UTIME_H)
check_include_file_cxx(glob.h PHAVE_GLOB_H)
check_include_file_cxx(dirent.h PHAVE_DIRENT_H)
check_include_file_cxx(drfftw.h PHAVE_DRFFTW_H)
check_include_file_cxx(sys/soundcard.h PHAVE_SYS_SOUNDCARD_H)
check_include_file_cxx(ucontext.h PHAVE_UCONTEXT_H)
check_include_file_cxx(linux/input.h PHAVE_LINUX_INPUT_H)

#/* Do we have RTTI (and <typeinfo>)? */
#$[cdefine HAVE_RTTI]

#/* Do we have Posix threads? */
#$[cdefine HAVE_POSIX_THREADS]

#/* Is the code being compiled with the Tau profiler's instrumentor? */
#$[cdefine USE_TAU]

#/* Define if needed to have 64-bit file i/o */
#$#[cdefine __USE_LARGEFILE64]

#/* Which memory allocation scheme should we use? */
#define USE_MEMORY_DLMALLOC
#define USE_MEMORY_PTMALLOC2
#define USE_MEMORY_MALLOC
#define USE_MEMORY_NOWRAPPERS
#if $[ALTERNATIVE_MALLOC]
#  #if $[and $[WIN32_PLATFORM], $[HAVE_THREADS], $[not $[SIMPLE_THREADS]]]
#    // A fast thread-safe alternative implementation, but which only
#    // seems to be a good choice on Windows.  (It crashes on Linux and
#    // isn't thread-safe on OSX).
#    #set USE_MEMORY_PTMALLOC2 1
#  #else
#    // A faster, but non-thread-safe, alternative implementation.
#    // When threading support is compiled in, we use a global mutex to
#    // protect it.
#    #set USE_MEMORY_DLMALLOC 1
#  #endif
##else
#  #if $[DO_MEMORY_USAGE]
#    // Redefine new and delete to malloc(), and also provide hooks for
#    // the benefit of the MemoryUsage class.
#    #set USE_MEMORY_MALLOC 1
#  #else
#    // Don't redefine new and delete at all.
#    #set USE_MEMORY_NOWRAPPERS 1
#  #endif
##endif
#$[cdefine USE_MEMORY_DLMALLOC]
#$[cdefine USE_MEMORY_PTMALLOC2]
#$[cdefine USE_MEMORY_MALLOC]
#$[cdefine USE_MEMORY_NOWRAPPERS]

#// To activate the DELETED_CHAIN macros.
#$[cdefine USE_DELETED_CHAIN]

#// If we are to build the native net interfaces.
#$[cdefine WANT_NATIVE_NET]

#/* Turn off warnings for using scanf and such */
#if $[eq $[USE_COMPILER],MSVC9]
        #print Will ignore CRT_SECURE warnings for MSVC9
#        $[cdefine _CRT_SECURE_NO_WARNINGS]
        # pragma warning( disable : 4996 4275 )
#endif


#/* Can we define a modern-style STL allocator? */
set(USE_STL_ALLOCATOR 1)

#/* Define to compile the plugin code. */
#$[cdefine HAVE_P3D_PLUGIN]

#$[cdefine BUILD_IPHONE]
#$[cdefine UNIVERSAL_BINARIES]

configure_file(dtool_config.h.in dtool_config.h)
install_headers(dtool_config.h)

add_subdirectory(src)

