#pragma once


class DebugMode
{
public:
    static void setDebugMode(bool state);
    static bool debugMode();

    DebugMode(int argc, char *argv[]);
private:
    static bool debugMode_;
};

