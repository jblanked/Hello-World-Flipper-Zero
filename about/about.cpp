#include "about/about.hpp"

HelloWorldAbout::HelloWorldAbout()
{
    // nothing to do
}

HelloWorldAbout::~HelloWorldAbout()
{
    free();
}

uint32_t HelloWorldAbout::callbackToSubmenu(void *context)
{
    UNUSED(context);
    return HelloWorldViewSubmenu;
}

void HelloWorldAbout::free()
{
    if (widget && viewDispatcherRef && *viewDispatcherRef)
    {
        view_dispatcher_remove_view(*viewDispatcherRef, HelloWorldViewAbout);
        widget_free(widget);
        widget = nullptr;
    }
}

bool HelloWorldAbout::init(ViewDispatcher **viewDispatcher, void *appContext)
{
    viewDispatcherRef = viewDispatcher;
    this->appContext = appContext;
    return easy_flipper_set_widget(&widget, HelloWorldViewAbout, "Simple C++ Flipper app\n\n\n\n\nwww.github.com/jblanked", callbackToSubmenu, viewDispatcherRef);
}
