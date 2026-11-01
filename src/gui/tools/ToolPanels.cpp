#include "gui/tools/ToolPanels.h"

#include "gui/Translation.h"

namespace flatcam::gui {

QStringList preprocessorNames() {
    return {"Berta_CNC",  "default",       "Default_no_M6",         "grbl_11",
            "GRBL_11_no_M6", "GRBL_laser", "ISEL_CNC",              "ISEL_ICP_CNC",
            "line_xyz",   "Marlin",        "Marlin_laser_FAN_pin",  "Marlin_laser_Spindle_pin",
            "Paste_1",    "Repetier",      "Roland_MDX_20",         "Toolchange_Custom",
            "Toolchange_Manual", "Toolchange_Probe_MACH3", "Z_laser"};
}

}  // namespace flatcam::gui
