#pragma once

#include <QtCore/qglobal.h>

#if defined(UPDATEDOWNLOADER_LIBRARY)
#  define UPDATEDOWNLOADER_EXPORT Q_DECL_EXPORT
#else
#  define UPDATEDOWNLOADER_EXPORT Q_DECL_IMPORT
#endif
