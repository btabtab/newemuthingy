#include <raylib.h>
#include <../MetaLauncher/RaylibHandling.h>
#include "old_emu_code.h"
int main()
{
    startup("VisEmu", 2500, 1000, getGlobalCamera(), n_frame_rate);
    SetCameraMode(*getGlobalCamera(), CAMERA_FREE);
    return runEmu();
}