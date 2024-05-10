#include "pseudo_input.hpp"

int main(int argc, char* argv[]) {
    Display* disp = XOpenDisplay(0);
    if (disp == NULL) {
        std::cerr << "Failed to obtain the display" << std::endl;
        exit(1);
    }

    Window root = XDefaultRootWindow(disp);

    Window win;
    int revert = 0;
    XGetInputFocus(disp, &win, &revert);

    XKeyEvent event = createKeyEvent(disp, win, root, true, DOWN, 0);

    XSendEvent(
        event.display,
        event.window,
        True,
        KeyPressMask,
        (XEvent*)&event
    );

    XCloseDisplay(disp);

    return 0;
}
