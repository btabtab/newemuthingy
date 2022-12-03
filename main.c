#include <raylib.h>
#include <../MetaLauncher/RaylibHandling.h>

#include <stdlib.h>
#include <stdio.h>

#include "old_emu_code.h"
#include "EmuPrinting.h"
int main()
{
    startup("VisEmu", 2500, 1000, getGlobalCamera(), n_frame_rate);
    // initVRAMColours();
    SetCameraMode(*getGlobalCamera(), CAMERA_FREE);
    return runEmu();
}