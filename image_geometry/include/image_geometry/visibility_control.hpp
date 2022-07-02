
#ifndef IMAGE_GEOMETRY__VISIBILITY_CONTROL_H_
#define IMAGE_GEOMETRY__VISIBILITY_CONTROL_H_

#if __cplusplus
extern "C"
{
#endif

// This logic was borrowed (then namespaced) from the examples on the gcc wiki:
//     https://gcc.gnu.org/wiki/Visibility

#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define IMAGE_GEOMETRY_EXPORT __attribute__ ((dllexport))
    #define IMAGE_GEOMETRY_IMPORT __attribute__ ((dllimport))
  #else
    #define IMAGE_GEOMETRY_EXPORT __declspec(dllexport)
    #define IMAGE_GEOMETRY_IMPORT __declspec(dllimport)
  #endif
  #ifdef IMAGE_GEOMETRY_BUILDING_DLL
    #define IMAGE_GEOMETRY_PUBLIC IMAGE_GEOMETRY_EXPORT
  #else
    #define IMAGE_GEOMETRY_PUBLIC IMAGE_GEOMETRY_IMPORT
  #endif
  #define IMAGE_GEOMETRY_PUBLIC_TYPE IMAGE_GEOMETRY_PUBLIC
  #define IMAGE_GEOMETRY_LOCAL
#else
  #define IMAGE_GEOMETRY_EXPORT __attribute__ ((visibility("default")))
  #define IMAGE_GEOMETRY_IMPORT
  #if __GNUC__ >= 4
    #define IMAGE_GEOMETRY_PUBLIC __attribute__ ((visibility("default")))
    #define IMAGE_GEOMETRY_LOCAL  __attribute__ ((visibility("hidden")))
  #else
    #define IMAGE_GEOMETRY_PUBLIC
    #define IMAGE_GEOMETRY_LOCAL
  #endif
  #define IMAGE_GEOMETRY_PUBLIC_TYPE
#endif

#if __cplusplus
}
#endif

#endif  // IMAGE_GEOMETRY__VISIBILITY_CONTROL_H_