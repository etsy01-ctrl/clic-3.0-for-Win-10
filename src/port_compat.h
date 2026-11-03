// Win32 replacements for Win16 conveniences used by the Clic 3.0 player.
#ifndef CLIC_PORT_COMPAT_H
#define CLIC_PORT_COMPAT_H

#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#ifndef far
#define far
#endif
#ifndef huge
#define huge
#endif
#ifndef _huge
#define _huge
#endif
#ifndef _export
#define _export
#endif

#define _fstrlen strlen
#define _fstrcpy strcpy
#define _fstrncpy strncpy
#define _fstrnicmp strnicmp
#define _fstricmp stricmp
#define _fstrchr strchr
#define _fstrstr strstr
#define _fmemset memset
#define _fmemcpy memcpy
#define _fmemmove memmove
#define farmalloc malloc
#define farfree free

#ifndef READ
#define READ 0
#endif
#ifndef OF_SEARCH
#define OF_SEARCH 0x0004
#endif
#ifndef GWW_ID
#define GWW_ID GWL_ID
#endif

inline POINT ClicPointFromLParam(LPARAM value)
{
  POINT point;
  point.x = (short)LOWORD(value);
  point.y = (short)HIWORD(value);
  return point;
}

#define MAKEPOINT(value) ClicPointFromLParam(value)
#define MoveTo(hdc, x, y) MoveToEx((hdc), (x), (y), NULL)
#define SetWindowExt(hdc, x, y) SetWindowExtEx((hdc), (x), (y), NULL)
#define SetViewportExt(hdc, x, y) SetViewportExtEx((hdc), (x), (y), NULL)
#define SetWindowOrg(hdc, x, y) SetWindowOrgEx((hdc), (x), (y), NULL)
#define SetBitmapDimension(bitmap, x, y) SetBitmapDimensionEx((bitmap), (x), (y), NULL)

#endif
