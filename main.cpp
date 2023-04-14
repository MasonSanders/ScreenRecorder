#include "ScreenRecorder.h"

int main(int argc, char* argv[])
{
    auto app = Glib::RefPtr<ScreenRecorder>(new ScreenRecorder());

    return app->run();
}