#ifndef Y_DEBUGGING_H
#define Y_DEBUGGING_H

#include "iostream"
//#define NDEBUG
//printf( "%s %s %d ", __FILE__, __FUNCTION__, __LINE__ );
// --> source 파일 이름, function 이름, line 번호를 출력해줍니다.

#ifndef NDEBUG
  #define dfp() printf( "//-----------------------------------------------------------\n// File : %s\n// Compile Time : %s %s\n//-----------------------------------------------------------\n", __FILE__, __DATE__, __TIME__)
  #define dp(fmt,...)  printf( fmt, __VA_ARGS__ )
  #define dlp(fmt,...) printf( "[%s %s %d] ", __FILE__, __FUNCTION__,__LINE__); printf( fmt, __VA_ARGS__ )
#else
  #define dfp()
  #define dp(fmt,...)
  #define dlp(fmt,...)
#endif

#endif //  !Y_DEBUGGING_H  //