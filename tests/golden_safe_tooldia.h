// ARCHIVO GENERADO por tests/tools/oracle_safe_tooldia.py - no editar a mano.
// Referencia: ToolNCC.find_optim_mp original (diámetro de herramienta seguro).
#pragma once
#include <vector>
namespace golden_safe_tooldia {
struct Case { const char* name; const char* gerber; bool isPath; bool ok; double dist; };
inline const std::vector<Case> kCases = {
{"mixed", "%FSLAX46Y46*%\n%MOMM*%\n%LPD*%\n%ADD10C,0.400000*%\n%ADD11R,1.200000X0.800000*%\n%ADD12O,1.000000X1.600000*%\n%ADD13C,1.000000*%\n%ADD14C,0.300000*%\nD10*\nX0Y0D02*\nX5000000Y0D01*\nX5000000Y3000000D01*\nD11*\nX0Y0D03*\nD13*\nX5000000Y3000000D03*\nD12*\nX10000000Y0D03*\nD10*\nX10000000Y0D02*\nX10000000Y4000000D01*\nX7000000Y4000000D01*\nD14*\nX0Y6000000D02*\nX3000000Y8000000D01*\nX6000000Y6000000D01*\nG36*\nX12000000Y0D02*\nX14000000Y0D01*\nX14000000Y2000000D01*\nX12000000Y2000000D01*\nX12000000Y0D01*\nG37*\nM02*\n", false, true, 1.5},
{"single_polygon", "%FSLAX46Y46*%\n%MOMM*%\n%LPD*%\n%ADD10C,0.300000*%\nD10*\nX0Y0D02*\nX4000000Y0D01*\nM02*\n", false, false, 0.0},
{"user_logo", "luz_de_noche/luz de noche Pascual-Logo.gbr", true, true, 0.0285},
{"user_b_cu", "luz_de_noche/luz de noche Pascual-B_Cu.gbr", true, true, 0.1994},
};
}  // namespace golden_safe_tooldia
