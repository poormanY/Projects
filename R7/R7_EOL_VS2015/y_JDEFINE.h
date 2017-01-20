////////////////////////////////////////
//       JDefine.h                    //
////////////////////////////////////////
#ifndef Y_JDEFINE_H
#define Y_JDEFINE_H

#ifndef  S16
#  define TOF          bool
#  define S8    signed char
#  define U8  unsigned char
#  define S08   signed char
#  define U08 unsigned char
#  define S16   signed short int
#  define U16 unsigned short int
#  define S32   signed int
#  define U32 unsigned int
#  define S64   signed __int64
#  define U64 unsigned __int64
#endif //S16

#ifndef  s16
#  define s8    signed char
#  define u8  unsigned char
#  define s08   signed char
#  define u08 unsigned char
#  define s16   signed short int
#  define u16 unsigned short int
#  define s32   signed int
#  define u32 unsigned int
#  define s64   signed __int64
#  define u64 unsigned __int64
#endif //s16

union ArbS16
{
   S16 data;
   U08 arByte[2];
};

union ArbU16
{
   U16 data;
   U08 arByte[2];
};

union ArbS32
{
   S32 data;
   U08 arByte[4];
};

union ArbU32
{
   U32 data;
   U08 arByte[4];
};

#ifndef  _T_
#  define _T_ 1
#  define _t_ 1
#  define _F_ 0
#  define _f_ 0
#endif //_T_

#ifndef  _ON_
#  define _ON_  1
#  define _on_  1
#  define _OFF_ 0
#  define _off_ 0
#endif //_ON_

#if   defined(DLL_EXPORT)
#     define DLL_EXP __declspec(dllexport)
#else
#  if defined(DLL_IMPORT)
#     define DLL_EXP __declspec(dllimport)
#  else
#     define DLL_EXP
#  endif
#endif

enum
{
   FILE_ANY    = 0,

   FILE_BIN    = 0x100,

   FILE_TEXT   = 0x200,
   FILE_HTM,
   FILE_CMD,

   FILE_BMP    = 0x300,
   FILE_PCX,
   FILE_GIF,
   FILE_JPG,

   FILE_VOC    = 0x400,
   FILE_WAV,
   FILE_MID,

   FILE_AVI    = 0x500,
};

#endif //  !Y_JDEFINE_H  //
