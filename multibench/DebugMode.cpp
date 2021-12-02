#include "DebugMode.h"

bool DebugMode::debugMode_ = false;

DebugMode::DebugMode(int argc, char *argv[])
{

}

void DebugMode::setDebugMode(bool state) {
    debugMode_ = state;
}

bool DebugMode::debugMode() {
    return debugMode_;
}
