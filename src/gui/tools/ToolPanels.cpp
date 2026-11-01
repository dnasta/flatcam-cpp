#include "gui/tools/ToolPanels.h"

#include "gui/Translation.h"

namespace flatcam::gui {

ToolInfo NccTool::info() { return {tr_("Non-Copper Clearing"), tr_("NCC Tool")}; }
ToolInfo PaintTool::info() { return {tr_("Paint Tool"), tr_("Paint Tool")}; }
ToolInfo DrillingTool::info() { return {tr_("Drilling Tool"), tr_("Drilling Tool")}; }
ToolInfo CutOutTool::info() { return {tr_("Cutout PCB"), tr_("Cutout Tool")}; }

}  // namespace flatcam::gui
