#include "run/run.hpp"
#include "app.hpp"

HelloWorldRun::HelloWorldRun()
{
    // nothing to do
}

HelloWorldRun::~HelloWorldRun()
{
    // nothing to do
}

void HelloWorldRun::updateDraw(Canvas *canvas)
{
    canvas_clear(canvas);
    canvas_draw_str(canvas, 0, 10, "Hello World Run!");
}

void HelloWorldRun::updateInput(InputEvent *event)
{
    if (event->key == InputKeyBack)
    {
        // return to menu
        shouldReturnToMenu = true;
    }
}
