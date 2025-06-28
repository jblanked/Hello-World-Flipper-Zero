#include "settings.hpp"
#include "app.hpp"

HelloWorldSettings::HelloWorldSettings()
{
    // nothing to do
}

HelloWorldSettings::~HelloWorldSettings()
{
    free();
}

void HelloWorldSettings::settings_item_selected_callback(void *context, uint32_t index)
{
    HelloWorldSettings *settings = (HelloWorldSettings *)context;
    settings->settingsItemSelected(index);
}

uint32_t HelloWorldSettings::callback_to_submenu(void *context)
{
    UNUSED(context);
    return HelloWorldViewSubmenu;
}

uint32_t HelloWorldSettings::callback_to_settings(void *context)
{
    UNUSED(context);
    return HelloWorldViewSettings;
}

bool HelloWorldSettings::init(ViewDispatcher **view_dispatcher, void *appContext)
{
    view_dispatcher_ref = view_dispatcher;
    this->appContext = appContext;

    if (!easy_flipper_set_variable_item_list(&variable_item_list, HelloWorldViewSettings,
                                             settings_item_selected_callback, callback_to_submenu, view_dispatcher, this))
    {
        return false;
    }

    variable_item_wifi_ssid = variable_item_list_add(variable_item_list, "WiFi SSID", 1, nullptr, nullptr);
    variable_item_wifi_pass = variable_item_list_add(variable_item_list, "WiFi Password", 1, nullptr, nullptr);

    variable_item_set_current_value_text(variable_item_wifi_ssid, "");
    variable_item_set_current_value_text(variable_item_wifi_pass, "");

    return true;
}

void HelloWorldSettings::free()
{
    // Free text input first
    free_text_input();

    if (variable_item_list && view_dispatcher_ref && *view_dispatcher_ref)
    {
        view_dispatcher_remove_view(*view_dispatcher_ref, HelloWorldViewSettings);
        variable_item_list_free(variable_item_list);
        variable_item_list = nullptr;
        variable_item_wifi_ssid = nullptr;
        variable_item_wifi_pass = nullptr;
    }
}

// Text input callback wrappers
void HelloWorldSettings::text_updated_ssid_callback(void *context)
{
    HelloWorldSettings *settings = (HelloWorldSettings *)context;
    settings->text_updated(TextInputWiFiSSID);
}

void HelloWorldSettings::text_updated_pass_callback(void *context)
{
    HelloWorldSettings *settings = (HelloWorldSettings *)context;
    settings->text_updated(TextInputWiFiPassword);
}

void HelloWorldSettings::text_updated(uint32_t view)
{
    // store the entered text
    strncpy(text_input_buffer.get(), text_input_temp_buffer.get(), text_input_buffer_size);

    // Ensure null-termination
    text_input_buffer[text_input_buffer_size - 1] = '\0';

    // app context
    HelloWorldApp *app = static_cast<HelloWorldApp *>(appContext);

    switch (view)
    {
    case TextInputWiFiSSID:
        if (variable_item_wifi_ssid)
        {
            variable_item_set_current_value_text(variable_item_wifi_ssid, text_input_buffer.get());
        }
        app->save_char("wifi_ssid", text_input_buffer.get());
        break;
    case TextInputWiFiPassword:
        if (variable_item_wifi_pass)
        {
            variable_item_set_current_value_text(variable_item_wifi_pass, text_input_buffer.get());
        }
        app->save_char("wifi_pass", text_input_buffer.get());
        break;
    default:
        break;
    }

    // switch to the settings view
    if (view_dispatcher_ref && *view_dispatcher_ref)
    {
        view_dispatcher_switch_to_view(*view_dispatcher_ref, HelloWorldViewSettings);
    }
}

bool HelloWorldSettings::init_text_input(uint32_t view)
{
    // check if already initialized
    if (text_input_buffer || text_input_temp_buffer)
    {
        FURI_LOG_E(TAG, "init_text_input: already initialized");
        return false;
    }

    // init buffers
    text_input_buffer_size = 128;
    if (!easy_flipper_set_buffer(reinterpret_cast<char **>(&text_input_buffer), text_input_buffer_size))
    {
        return false;
    }
    if (!easy_flipper_set_buffer(reinterpret_cast<char **>(&text_input_temp_buffer), text_input_buffer_size))
    {
        return false;
    }

    // app context
    HelloWorldApp *app = static_cast<HelloWorldApp *>(appContext);
    char loaded[256];

    if (view == TextInputWiFiSSID)
    {
        if (app->load_char("wifi_ssid", loaded, sizeof(loaded)))
        {
            strncpy(text_input_temp_buffer.get(), loaded, text_input_buffer_size);
        }
        else
        {
            text_input_temp_buffer[0] = '\0'; // Ensure empty if not loaded
        }
        text_input_temp_buffer[text_input_buffer_size - 1] = '\0'; // Ensure null-termination
        return easy_flipper_set_text_input(&text_input, HelloWorldViewTextInput,
                                           "Enter SSID", text_input_temp_buffer.get(), text_input_buffer_size,
                                           text_updated_ssid_callback, callback_to_settings, view_dispatcher_ref, this);
    }
    else if (view == TextInputWiFiPassword)
    {
        if (app->load_char("wifi_pass", loaded, sizeof(loaded)))
        {
            strncpy(text_input_temp_buffer.get(), loaded, text_input_buffer_size);
        }
        else
        {
            text_input_temp_buffer[0] = '\0'; // Ensure empty if not loaded
        }
        text_input_temp_buffer[text_input_buffer_size - 1] = '\0'; // Ensure null-termination
        return easy_flipper_set_text_input(&text_input, HelloWorldViewTextInput,
                                           "Enter Password", text_input_temp_buffer.get(), text_input_buffer_size,
                                           text_updated_pass_callback, callback_to_settings, view_dispatcher_ref, this);
    }
    return false;
}

void HelloWorldSettings::free_text_input()
{
    if (text_input && view_dispatcher_ref && *view_dispatcher_ref)
    {
        view_dispatcher_remove_view(*view_dispatcher_ref, HelloWorldViewTextInput);
        text_input_free(text_input);
        text_input = nullptr;
    }
    text_input_buffer.reset();
    text_input_temp_buffer.reset();
}

bool HelloWorldSettings::start_text_input(uint32_t view)
{
    free_text_input();
    if (!init_text_input(view))
    {
        FURI_LOG_E(TAG, "Failed to initialize text input for view %lu", view);
        return false;
    }
    if (view_dispatcher_ref && *view_dispatcher_ref)
    {
        view_dispatcher_switch_to_view(*view_dispatcher_ref, HelloWorldViewTextInput);
        return true;
    }
    else
    {
        FURI_LOG_E(TAG, "View dispatcher reference is null or invalid");
        return false;
    }
}

void HelloWorldSettings::settingsItemSelected(uint32_t index)
{
    start_text_input(index);
}
