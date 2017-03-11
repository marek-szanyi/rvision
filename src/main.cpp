#include "easylogging++.h"
#include "application.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, const char **argv) {
    rvision::sys::application app;
    return app.run(argc, argv);
}