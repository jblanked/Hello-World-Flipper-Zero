#pragma once
#include "easy_flipper/easy_flipper.h"

class HelloWorldApp;

typedef enum
{
    TextInputWiFiSSID = 0,
    TextInputWiFiPassword = 1,
} TextInputChoice;

class HelloWorldSettings
{
private:
    VariableItemList *variable_item_list = nullptr;
    VariableItem *variable_item_wifi_ssid = nullptr;
    VariableItem *variable_item_wifi_pass = nullptr;
    ViewDispatcher **view_dispatcher_ref = nullptr;
    void *appContext = nullptr;

    // Text input related members
    TextInput *text_input = nullptr;
    uint32_t text_input_buffer_size = 128;
    std::unique_ptr<char[]> text_input_buffer;
    std::unique_ptr<char[]> text_input_temp_buffer;

    // Static callback wrappers
    static void settings_item_selected_callback(void *context, uint32_t index);
    static uint32_t callback_to_submenu(void *context);
    static uint32_t callback_to_settings(void *context);
    static void text_updated_ssid_callback(void *context);
    static void text_updated_pass_callback(void *context);

    // Text input methods
    void text_updated(uint32_t view);
    bool init_text_input(uint32_t view);
    void free_text_input();
    bool start_text_input(uint32_t view);

public:
    HelloWorldSettings();
    ~HelloWorldSettings();

    bool init(ViewDispatcher **view_dispatcher, void *appContext);
    void free();
    void settingsItemSelected(uint32_t index);

    // Getters for variable items
    VariableItem *getWiFiSSIDItem() const { return variable_item_wifi_ssid; }
    VariableItem *getWiFiPassItem() const { return variable_item_wifi_pass; }
};
