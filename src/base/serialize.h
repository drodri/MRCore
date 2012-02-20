/////////////////////////////////////////////////////////////////////////////
// Serialize.h header file
//
// This file is part of Jabon software
// Copyright (C) 2008. Diego Rodriguez-Losada. All rights reserved
// Use in commercial applications requires written permission
// This software is provided "as is", with no warranty.
/////////////////////////////////////////////////////////////////////////////

#if !defined(_SERIALIZE_INCLUDED_)
#define _SERIALIZE_INCLUDED_

#define writeChar(x, y, z)     {x[y++] = z;}
#define writeShort(x, y, z)     {*((unsigned short *)((char *)&x[y])) = z; y += 2;}
#define writeLong(x, y, z)      {*((unsigned long  *)((char *)&x[y])) = z; y += 4;}
#define writeFloat(x, y, z)     {*((float  *)((char *)&x[y])) = z; y += 4;}
#define writeDouble(x, y, z)    {*((double *)((char *)&x[y])) = z; y += 8;}

#define readChar(x, y, z)      {z = x[y++];}
#define readShort(x, y, z)      {z = *(unsigned short *)((char *)&x[y]); y += 2;}
#define readLong(x, y, z)       {z = (*(unsigned long  *)((char *)&x[y])); y += 4;}
#define readFloat(x, y, z)      {z = (*(float  *)((char *)&x[y])); y += 4;}
#define readDouble(x, y, z)     {z = (*(double *)((char *)&x[y])); y += 8;}

#endif // !defined(AFX_SOCKETNODE_H__895095F5_AD65_4FF5_973F_0DD16E9BC2FE__INCLUDED_)
