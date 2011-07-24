#ifndef NALL_PLATFORM_HPP
#define NALL_PLATFORM_HPP

#if defined(_WIN32)
  //minimum version needed for _wstat64, etc
  #undef  __MSVCRT_VERSION__
  #define __MSVCRT_VERSION__ 0x0601
#endif

#include <nall/utf8.hpp>

//=========================
//standard platform headers
//=========================

#include <limits>

#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>

#if defined(_WIN32)
  #include <io.h>
  #include <direct.h>
  #include <shlobj.h>
  #include <wchar.h>
  #undef interface
  #define dllexport __declspec(dllexport)
#else
  #include <unistd.h>
  #include <pwd.h>
  #define dllexport
#endif

//==================
//warning supression
//==================

//Visual C++
#if defined(_MSC_VER)
  //disable libc "deprecation" warnings
  #pragma warning(disable:4996)
#endif

//================
//POSIX compliance
//================

#if defined(_MSC_VER)
  #define PATH_MAX  _MAX_PATH
  #define va_copy(dest, src)  ((dest) = (src))
#endif

#if defined(_WIN32)
  #define getcwd      _getcwd
  #define ftruncate   _chsize
  #define mkdir(n, m) _wmkdir(nall::utf16_t(n))
  #define putenv      _putenv
  #define rmdir       _rmdir
  #define usleep(n)   Sleep(n / 1000)
  #define vsnprintf   _vsnprintf
  static int close(int sock) { return closesocket(sock); }
#endif

//================
//inline expansion
//================

#if defined(__GNUC__)
  #define noinline      __attribute__((noinline))
  #define inline        inline
  #define alwaysinline  inline __attribute__((always_inline))
#elif defined(_MSC_VER)
  #define noinline      __declspec(noinline)
  #define inline        inline
  #define alwaysinline  inline __forceinline
#else
  #define noinline
  #define inline        inline
  #define alwaysinline  inline
#endif

//=========================
//file system functionality
//=========================

#if defined(_WIN32)
  inline char* realpath(const char *filename, char *resolvedname) {
    wchar_t fn[_MAX_PATH] = L"";
    _wfullpath(fn, nall::utf16_t(filename), _MAX_PATH);
    strcpy(resolvedname, nall::utf8_t(fn));
    return resolvedname;
  }

  inline char* userpath(char *path) {
    wchar_t fp[_MAX_PATH] = L"";
    SHGetFolderPathW(0, CSIDL_APPDATA | CSIDL_FLAG_CREATE, 0, 0, fp);
    strcpy(path, nall::utf8_t(fp));
    return path;
  }

  inline char* getcwd(char *path) {
    wchar_t fp[_MAX_PATH] = L"";
    _wgetcwd(fp, _MAX_PATH);
    strcpy(path, nall::utf8_t(fp));
    return path;
  }
#else
  //realpath() already exists

  inline char* userpath(char *path) {
    *path = 0;
    struct passwd *userinfo = getpwuid(getuid());
    if(userinfo) strcpy(path, userinfo->pw_dir);
    return path;
  }

  inline char *getcwd(char *path) {
    return getcwd(path, PATH_MAX);
  }
#endif

#endif

