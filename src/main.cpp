#include "shared.hpp"
#include "app/app.hpp"
#include "factory/unique_id_resolver.hpp"

int main() {
    App app;
    app.run();

    debug_log("DEBUG: main() 'Bye Bye'");

    return 0;
}
