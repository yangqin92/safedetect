#include "stdafx.h"

#define ST(x) #x
#define STR(x) ST(x)

#define CO(x,y) x##y
#define CON(x,y) CO(x,y)//加这层宏的用意是把所有宏的参数在这层里全部展开

#define OPENCVLIBROOT D:/opencv/build/x86/vc12/lib/
#define OPENCVLIB(x) STR(CON(OPENCVLIBROOT,x))

#define PROJECTROOT C:/Users/Administrator/Documents/Visual Studio 2015/Projects/MFC_safedetect/MFC_safedetect/
#define PROJECTLIB(x) STR(CON(PROJECTROOT,x))

#pragma comment(lib, OPENCVLIB(opencv_contrib2412d.lib))
#pragma comment(lib, OPENCVLIB(opencv_core2412d.lib))
#pragma comment(lib, OPENCVLIB(opencv_features2d2412d.lib))
#pragma comment(lib, OPENCVLIB(opencv_flann2412d.lib))
#pragma comment(lib, OPENCVLIB(opencv_gpu2412d.lib))
#pragma comment(lib, OPENCVLIB(opencv_highgui2412d.lib))
#pragma comment(lib, OPENCVLIB(opencv_imgproc2412d.lib))
#pragma comment(lib, OPENCVLIB(opencv_legacy2412d.lib))
#pragma comment(lib, OPENCVLIB(opencv_ml2412d.lib))
#pragma comment(lib, OPENCVLIB(opencv_nonfree2412d.lib))
#pragma comment(lib, OPENCVLIB(opencv_objdetect2412d.lib))
#pragma comment(lib, OPENCVLIB(opencv_ocl2412d.lib))
#pragma comment(lib, OPENCVLIB(opencv_photo2412d.lib))
#pragma comment(lib, OPENCVLIB(opencv_stitching2412d.lib))
#pragma comment(lib, OPENCVLIB(opencv_superres2412d.lib))
#pragma comment(lib, OPENCVLIB(opencv_ts2412d.lib))
#pragma comment(lib, OPENCVLIB(opencv_video2412d.lib))
#pragma comment(lib, OPENCVLIB(opencv_videostab2412d.lib))
