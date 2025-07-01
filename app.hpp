#pragma once
#include "font/font.h"
#include "easy_flipper/easy_flipper.h"
#include "flipper_http/flipper_http.h"
#include "run/run.hpp"
#include "settings/settings.hpp"
#include "about/about.hpp"

#define TAG "Hello World"
#define APP_ID "hello_world"

typedef enum
{
    HelloWorldSubmenuRun = 0,
    HelloWorldSubmenuAbout = 1,
    HelloWorldSubmenuSettings = 2,
} HelloWorldSubmenuIndex;

typedef enum
{
    HelloWorldViewMain = 0,
    HelloWorldViewSubmenu = 1,
    HelloWorldViewAbout = 2,
    HelloWorldViewSettings = 3,
    HelloWorldViewTextInput = 4,
} HelloWorldView;

class HelloWorldApp
{
private:
    Submenu *submenu = nullptr;
    FlipperHTTP *flipperHttp = nullptr;

    // Run class instance
    std::unique_ptr<HelloWorldRun> run;

    // Settings class instance
    std::unique_ptr<HelloWorldSettings> settings;

    // About class instance
    std::unique_ptr<HelloWorldAbout> about;

    // Timer for game updates
    FuriTimer *timer = nullptr;

    // Static callback functions
    static uint32_t callback_exit_app(void *context);
    static void submenu_choices_callback(void *context, uint32_t index);
    static void settings_item_selected_callback(void *context, uint32_t index);
    static void timerCallback(void *context);

    // Member functions
    void callbackSubmenuChoices(uint32_t index);
    void settingsItemSelected(uint32_t index);

    void createAppDataPath();

public:
    bool load_char(const char *path_name, char *value, size_t value_size); // load a string from storage
    bool save_char(const char *path_name, const char *value);              // save a string to storage
    ViewDispatcher *viewDispatcher = nullptr;
    Gui *gui = nullptr;
    ViewPort *viewPort = nullptr;
    //
    bool isBoardConnected(); // check if the board is connected

    size_t getBytesReceived() const noexcept { return flipperHttp ? flipperHttp->bytes_received : 0; }
    size_t getContentLength() const noexcept { return flipperHttp ? flipperHttp->content_length : 0; }
    HTTPState getHttpState() const noexcept { return flipperHttp ? flipperHttp->state : INACTIVE; }
    bool setHttpState(HTTPState state = IDLE) noexcept
    {
        if (flipperHttp)
        {
            flipperHttp->state = state;
            return true;
        }
        return false;
    }

    FuriString *httpRequest(
        const char *url,
        HTTPMethod method = GET,
        const char *headers = "{\"Content-Type\": \"application/json\"}",
        const char *payload = nullptr);

    // for this one, check the HttpState to see if the request is finished
    // and then check the location
    // I think I'll add the loading view/animation to the one above
    // so this one is just like we used in the old apps
    // saveLocation is just a filename (like "response.json" or "data.json")
    bool httpRequestAsync(
        const char *saveLocation,
        const char *url,
        HTTPMethod method = GET,
        const char *headers = "{\"Content-Type\": \"application/json\"}",
        const char *payload = nullptr);

    HelloWorldApp();
    ~HelloWorldApp();

    void runDispatcher();

    static void viewPortDraw(Canvas *canvas, void *context);
    static void viewPortInput(InputEvent *event, void *context);
};