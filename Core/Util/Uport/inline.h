#ifndef _INLINE_H_
#define _INLINE_H_

#undef INLINE

#if defined(__cplusplus)
#  define INLINE inline
#else
#  if defined(_WIN32) || defined(__GNUC__)
#    define INLINE __inline
#  else 
#    if defined(LINUX)
#      define INLINE inline
#    else
#      define INLINE
#    endif
#  endif
#endif

#endif /* _INLINE_H_ */

