#ifndef RUNTIME_ASSOCIATION_H
#define RUNTIME_ASSOCIATION_H
#include "w.h"
LONG registerPccAssociation(HKEY root, const char *exePath);
BOOL associatePccFiles(HWND owner);
#endif
