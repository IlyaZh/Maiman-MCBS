#include "DebugMode.h"
#include <QtGlobal>

bool DebugMode::debugMode_ = false;

DebugMode::DebugMode(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);
}

void DebugMode::setDebugMode(bool state) {
    debugMode_ = state;
}

bool DebugMode::debugMode() {
    return debugMode_;
}
