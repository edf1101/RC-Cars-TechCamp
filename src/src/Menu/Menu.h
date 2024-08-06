#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include <U8g2lib.h>
#include <functional>

#define ENC_A 4
#define ENC_B 17
#define ENC_SW 2

#define CLOCK_PIN 22
#define DATA_PIN 27

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

class MenuItem // Contains the structure of the any menu item which can be menus themselves
{
public:
    char itemName[17];
    MenuItem *parentItem = NULL; // contains the parent of this menu item - NULL for the root
    MenuItem *childItem = NULL; // links to first child item for submenus
    MenuItem *prevItem = NULL; // links to the previous menu item in this menu - NULL if it's the first
    MenuItem *nextItem = NULL; // links to the next menu item in this menu - NULL if it's the last
    int minVar, maxVar, defaultVar, incVar; // used for parameters to set the range, default and increment
    MenuItem(
             std::function<void(const char *, int)> _callback, const char *thisName, MenuItem *thisParent = NULL, MenuItem *thisChild = NULL, MenuItem *thisPrev = NULL,
             MenuItem *thisNext = NULL, int thisMin = 0, int thisMax = 0, int thisDefault = 0, int thisInc = 0);

    int addItem(const char *menuName, const char *thisName, int minVal = 0, int maxVal = 0, int defaultVal = 0,
                int increment = 1);

    MenuItem *getItem(const char *thisName);

    void activate();

    void (*menuFunction)(const char *itemName, int value);

    std::function<void(const char *, int)> callback;
};

void menuService();

void updateMenu();

#endif // MENU_H
