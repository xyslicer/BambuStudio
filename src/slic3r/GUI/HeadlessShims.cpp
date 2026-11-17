// Headless CLI only (SLIC3R_HEADLESS). libslic3r (NSVGUtils, Format/svg) uses
// nanosvg, whose implementation the GUI build compiles in BitmapCache.cpp.
#define NANOSVG_IMPLEMENTATION
#include "nanosvg/nanosvg.h"
