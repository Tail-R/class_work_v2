#include "shared.hpp"
#include "app/app.hpp"

int main() {
    App app;
    app.run();

    debug_log("DEBUG: main() 'Bye Bye'");

    return 0;
}
