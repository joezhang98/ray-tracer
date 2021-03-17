#ifndef UTIL_STB_IMAGE_H
#define UTIL_STB_IMAGE_H

/* Disable pedantic warnings for the external library. */
#ifdef _MSC_VER
    #pragma warning (push, 0)
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

/* Restore warning levels. */
#ifdef _MSC_VER
    #pragma warning (pop)
#endif

#endif 
