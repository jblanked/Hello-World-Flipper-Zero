#pragma once
#include <gui/gui.h>

class Loading
{
public:
    Loading(Canvas *canvas);
    //
    void animate();
    void stop();
    //
    void setText(const char *text) { currentText = text; }
    //
    uint16_t getTimeElapsed() { return timeElapsed; }

private:
    Canvas *canvas = nullptr;
    void drawSpinner();
    uint16_t spinnerPosition;
    uint16_t timeElapsed;
    uint16_t timeStart;
    bool animating = false;
    const char *currentText = "Loading...";
};