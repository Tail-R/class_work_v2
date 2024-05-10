#include <iostream>
#include <chrono>

#include <X11/Xlib.h>
#include <X11/keysym.h>

#define UP XK_Up
#define DOWN XK_Down
#define LEFT XK_Left
#define RIGHT XK_Right

XKeyEvent createKeyEvent(
    Display* disp,
    Window& win,
    Window& win_root,
    bool press,
    int keycode,
    int modifiers
    ) {

    XKeyEvent e;

    e.display       = disp;
    e.window        = win;
    e.root          = win_root;
    e.subwindow     = None;
    e.time          = CurrentTime;
    e.x             = 1;
    e.y             = 1;
    e.x_root        = 1;
    e.y_root        = 1;
    e.same_screen   = True;
    e.keycode       = XKeysymToKeycode(disp, keycode);
    e.state         = modifiers;
    e.type          = press ? KeyPress : KeyRelease;

    return e;
}
