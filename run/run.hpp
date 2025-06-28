#pragma once
#include "easy_flipper/easy_flipper.h"

class HelloWorldApp;

class HelloWorldRun
{
    bool shouldReturnToMenu = false; // Flag to signal return to menu
public:
    HelloWorldRun();
    ~HelloWorldRun();
    //
    bool isActive() const { return shouldReturnToMenu == false; } // Check if the run is active
    void updateDraw(Canvas *canvas);                              // update and draw the run
    void updateInput(InputEvent *event);                          // update input for the run
};
