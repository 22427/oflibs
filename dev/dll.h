#ifndef OFL_DLL_PUBLIC

#if defined _WIN32 || defined __CYGWIN__
#ifdef PT_BUILDING_DLL
#ifdef __GNUC__
#define OFL_DLL_PUBLIC __attribute__ ((dllexport))
#else
#define OFL_DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
#endif
#else
#ifdef __GNUC__
#define OFL_DLL_PUBLIC __attribute__ ((dllimport))
#else
#define OFL_DLL_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
#endif
#endif
#define OFL_DLL_LOCAL
#else
#if __GNUC__ >= 4
#define OFL_DLL_PUBLIC __attribute__ ((visibility ("default")))
#define OFL_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define OFL_DLL_PUBLIC
#define OFL_DLL_LOCAL
#endif
#endif

#endif
