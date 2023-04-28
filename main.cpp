#include "ScreenRecorder.h"

int main(int argc, char* argv[])
{
    // main returns the exit state of the application.
    auto app = Glib::RefPtr<Screen_Recorder>(new Screen_Recorder());

    return app->run(argc, argv);
}