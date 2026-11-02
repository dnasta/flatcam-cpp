// ARCHIVO GENERADO por tools/gen_pp_templates.py - no editar a mano.
// Plantillas multilínea de los postprocesadores originales de FlatCAM 2024.4, tal cual (con sus espacios).
#pragma once

namespace flatcam::pp_templates {

inline const char* const kBerta_CNC_toolchange_code[] = {
    R"PPT(
M5
G00 Z{z_toolchange}
G00 X{x_toolchange} Y{y_toolchange}                
T{tool}
M6
(MSG, Change to Tool Dia = {toolC} ||| Total drills for tool T{tool} = {t_drills})
M0
)PPT",
    R"PPT(
M5       
G00 Z{z_toolchange}
T{tool}
M6
(MSG, Change to Tool Dia = {toolC} ||| Total drills for tool T{tool} = {t_drills})
M0)PPT",
    R"PPT(
M5
G00 Z{z_toolchange}
G00 X{x_toolchange} Y{y_toolchange}
T{tool}
M6    
(MSG, Change to Tool Dia = {toolC})
M0)PPT",
    R"PPT(
M5
G00 Z{z_toolchange}
T{tool}
M6    
(MSG, Change to Tool Dia = {toolC})
M0)PPT",
};

inline const char* const kDefault_no_M6_toolchange_code[] = {
    R"PPT(
M5
G00 Z{z_toolchange}
T{tool}
G00 X{x_toolchange} Y{y_toolchange}                
(MSG, Change to Tool Dia = {toolC} ||| Total drills for tool T{tool} = {t_drills})
M0
G00 Z{z_toolchange}
		)PPT",
    R"PPT(
M5       
G00 Z{z_toolchange}
T{tool}
(MSG, Change to Tool Dia = {toolC} ||| Total drills for tool T{tool} = {t_drills})
M0
G00 Z{z_toolchange}
		)PPT",
    R"PPT(
M5
G00 Z{z_toolchange}
G00 X{x_toolchange} Y{y_toolchange}
T{tool}
(MSG, Change to Tool Dia = {toolC})
M0
G00 Z{z_toolchange}
		)PPT",
    R"PPT(
M5
G00 Z{z_toolchange}
T{tool}
(MSG, Change to Tool Dia = {toolC})
M0
G00 Z{z_toolchange}
		)PPT",
};

inline const char* const kGRBL_11_no_M6_toolchange_code[] = {
    R"PPT(
M5             
G00 Z{z_toolchange}
G00 X{x_toolchange} Y{y_toolchange}                
T{tool}
(MSG, Change to Tool Dia = {toolC} ||| Total drills for tool T{tool} = {t_drills})
M0
G00 Z{z_toolchange}
		)PPT",
    R"PPT(
M5             
G00 Z{z_toolchange}               
T{tool}
(MSG, Change to Tool Dia = {toolC} ||| Total drills for tool T{tool} = {t_drills})
M0
G00 Z{z_toolchange}
		)PPT",
    R"PPT(
M5             
G00 Z{z_toolchange}
G00 X{x_toolchange} Y{y_toolchange}                
T{tool}
(MSG, Change to Tool Dia = {toolC})
M0
G00 Z{z_toolchange}
		)PPT",
    R"PPT(
M5             
G00 Z{z_toolchange}              
T{tool}
(MSG, Change to Tool Dia = {toolC})
M0
G00 Z{z_toolchange}
		)PPT",
};

inline const char* const kISEL_CNC_toolchange_code[] = {
    R"PPT(
M05       
T{tool}
M06
(MSG, Change to Tool Dia = {toolC} ||| Total drills for tool T{tool} = {t_drills})
M01)PPT",
    R"PPT(
M05
T{tool}
M06    
(MSG, Change to Tool Dia = {toolC})
M01)PPT",
};

inline const char* const kISEL_ICP_CNC_toolchange_code[] = {
    R"PPT(GETTOOL {tool}
; Changed to Tool Dia = {toolC})PPT",
    R"PPT(GETTOOL {tool}
; Changed to Tool Dia = {toolC})PPT",
};

inline const char* const kMarlin_toolchange_code[] = {
    R"PPT(
M5
G0 Z{z_toolchange}
G0 X{x_toolchange} Y{y_toolchange}                
T{tool}
M6
;MSG, Change to Tool Dia = {toolC}, Total drills for tool T{tool} = {t_drills}
M0
G0 Z{z_toolchange}
)PPT",
    R"PPT(
M5
G0 Z{z_toolchange}
T{tool}
M6
;MSG, Change to Tool Dia = {toolC}, Total drills for tool T{tool} = {t_drills}
M0
G0 Z{z_toolchange}
)PPT",
    R"PPT(
M5
G0 Z{z_toolchange}
G0 X{x_toolchange} Y{y_toolchange}
T{tool}
M6    
;MSG, Change to Tool Dia = {toolC}
M0
G0 Z{z_toolchange}
)PPT",
    R"PPT(
M5
G0 Z{z_toolchange}
T{tool}
M6    
;MSG, Change to Tool Dia = {toolC}
M0
G0 Z{z_toolchange}
)PPT",
};

inline const char* const kPaste_1_toolchange_code[] = {
    R"PPT(
G00 Z{z_toolchange}
G00 X{x_toolchange} Y{y_toolchange}
T{tool}
M6    
(MSG, Change to Tool with Nozzle Dia = {toolC})
M0
G00 Z{z_toolchange}
)PPT",
    R"PPT(
G00 Z{z_toolchange}
T{tool}
M6    
(MSG, Change to Tool with Nozzle Dia = {toolC})
M0
G00 Z{z_toolchange}
)PPT",
};

inline const char* const kRepetier_toolchange_code[] = {
    R"PPT(
G0 Z{z_toolchange}
G0 X{x_toolchange} Y{y_toolchange}                
M84
@pause Change to Tool Dia = {toolC}, Total drills for tool T{tool} = {t_drills}
G0 Z{z_toolchange}
)PPT",
    R"PPT(
G0 Z{z_toolchange}
M84
@pause Change to Tool Dia = {toolC}, Total drills for tool T{tool} = {t_drills}
G0 Z{z_toolchange}
)PPT",
    R"PPT(
G0 Z{z_toolchange}
G0 X{x_toolchange} Y{y_toolchange}
M84
@pause Change to tool T{tool} with Tool Dia = {toolC}
G0 Z{z_toolchange}
)PPT",
    R"PPT(
G0 Z{z_toolchange}
M84
@pause Change to tool T{tool} with Tool Dia = {toolC}
G0 Z{z_toolchange}
)PPT",
};

inline const char* const kToolchange_Manual_toolchange_code[] = {
    R"PPT(
M5
G00 Z{z_toolchange}
T{tool} 
G00 X{x_toolchange} Y{y_toolchange} 
(MSG, Change to Tool Dia = {toolC} ||| Total drills for tool T{tool} = {t_drills})
M0
G01 Z0
(MSG, Adjust the tool T{tool} to touch the material and then tighten it slightly.)
M0
G00 Z{z_toolchange}
(MSG, Now the tool can be tightened more securely.)
M0
(MSG, Drilling with Tool Dia = {toolC} ||| Total drills for tool T{tool} = {t_drills})
)PPT",
    R"PPT(
M5 
G00 Z{z_toolchange}
T{tool} 
(MSG, Change to Tool Dia = {toolC} ||| Total drills for tool T{tool} = {t_drills})
M0
G01 Z0
(MSG, Adjust the tool T{tool} to touch the material and then tighten it slightly.)
M0
G00 Z{z_toolchange}
(MSG, Now the tool can be tightened more securely.)
M0
(MSG, Milling with Tool Dia = {toolC} ||| Total drills for tool T{tool} = {t_drills})
)PPT",
    R"PPT(
M5
G00 Z{z_toolchange}
T{tool}
G00 X{x_toolchange}Y{y_toolchange}    
(MSG, Change to Tool Dia = {toolC})
M0
G01 Z0
(MSG, Adjust the tool T{tool} to touch the material and then tighten it slightly.)
M0
G00 Z{z_toolchange}
(MSG, Now the tool can be tightened more securely.)
M0
)PPT",
    R"PPT(
M5  
G00 Z{z_toolchange}
T{tool}
(MSG, Change to Tool Dia = {toolC})
M0
G01 Z0
(MSG, Adjust the tool T{tool} to touch the material and then tighten it slightly.)
M0
G00 Z{z_toolchange}
(MSG, Now the tool can be tightened more securely.)
M0
)PPT",
};

inline const char* const kToolchange_Probe_MACH3_toolchange_code[] = {
    R"PPT(  
M5             
T{tool}
M6
G00 Z{z_toolchange}
G00 X{x_toolchange} Y{y_toolchange}
(MSG, Change to Tool Dia = {toolC} ||| CONNECT THE PROBE ||| Drills for this tool = {t_drills})
M0
F{feedrate_probe}
G31 Z{z_pdepth}
G92 Z0
G00 Z{z_in_between}
F{feedrate_probe_slow}
G31 Z{z_pdepth}
G92 Z0
G00 Z{z_move}
(MSG, Remove any clips or other devices used for probing. CNC work is resuming ...)
M0
)PPT",
    R"PPT(
M5
T{tool}
M6
G00 Z{z_toolchange}
(MSG, Change to Tool Dia = {toolC} ||| CONNECT THE PROBE ||| Drills for this tool = {t_drills})
M0
F{feedrate_probe}
G31 Z{z_pdepth}
G92 Z0
G00 Z{z_in_between}
F{feedrate_probe_slow}
G31 Z{z_pdepth}
G92 Z0
G00 Z{z_move}
(MSG, Remove any clips or other devices used for probing. CNC work is resuming ...)
M0
)PPT",
    R"PPT(
M5
T{tool}
M6
G00 Z{z_toolchange}
G00 X{x_toolchange} Y{y_toolchange}
(MSG, Change to Tool Dia = {toolC} ||| CONNECT THE PROBE)
M0
F{feedrate_probe}
G31 Z{z_pdepth}
G92 Z0
G00 Z{z_in_between}
F{feedrate_probe_slow}
G31 Z{z_pdepth}
G92 Z0
G00 Z{z_move}
(MSG, Remove any clips or other devices used for probing. CNC work is resuming ...)
M0
)PPT",
    R"PPT(
M5
T{tool}
M6
G00 Z{z_toolchange}
(MSG, Change to Tool Dia = {toolC} ||| CONNECT THE PROBE)
M0
F{feedrate_probe}
G31 Z{z_pdepth}
G92 Z0
G00 Z{z_in_between}
F{feedrate_probe_slow}
G31 Z{z_pdepth}
G92 Z0
G00 Z{z_move}
(MSG, Remove any clips or other devices used for probing. CNC work is resuming ...)
M0
)PPT",
};

inline const char* const kdefault_toolchange_code[] = {
    R"PPT(
M5
G00 Z{z_toolchange}
T{tool}
G00 X{x_toolchange} Y{y_toolchange}                
M6
(MSG, Change to Tool Dia = {toolC} ||| Total drills for tool T{tool} = {t_drills})
M0
G00 Z{z_toolchange}
)PPT",
    R"PPT(
M5       
G00 Z{z_toolchange}
T{tool}
M6
(MSG, Change to Tool Dia = {toolC} ||| Total drills for tool T{tool} = {t_drills})
M0
G00 Z{z_toolchange}
)PPT",
    R"PPT(
M5
G00 Z{z_toolchange}
G00 X{x_toolchange} Y{y_toolchange}
T{tool}
M6    
(MSG, Change to Tool Dia = {toolC})
M0
G00 Z{z_toolchange}
)PPT",
    R"PPT(
M5
G00 Z{z_toolchange}
T{tool}
M6    
(MSG, Change to Tool Dia = {toolC})
M0
G00 Z{z_toolchange}
)PPT",
};

inline const char* const kgrbl_11_toolchange_code[] = {
    R"PPT(
M5             
G00 Z{z_toolchange}
G00 X{x_toolchange} Y{y_toolchange}                
T{tool}
M6
(MSG, Change to Tool Dia = {toolC} ||| Total drills for tool T{tool} = {t_drills})
M0
G00 Z{z_toolchange}
)PPT",
    R"PPT(
M5             
G00 Z{z_toolchange}               
T{tool}
M6
(MSG, Change to Tool Dia = {toolC} ||| Total drills for tool T{tool} = {t_drills})
M0
G00 Z{z_toolchange}
)PPT",
    R"PPT(
M5             
G00 Z{z_toolchange}
G00 X{x_toolchange} Y{y_toolchange}                
T{tool}
M6
(MSG, Change to Tool Dia = {toolC})
M0
G00 Z{z_toolchange}
)PPT",
    R"PPT(
M5             
G00 Z{z_toolchange}              
T{tool}
M6
(MSG, Change to Tool Dia = {toolC})
M0
G00 Z{z_toolchange}
)PPT",
};

inline const char* const kline_xyz_toolchange_code[] = {
    R"PPT(
M5      
G00 X{x_toolchange} Y{x_toolchange} Z{z_toolchange}
T{tool}
M6
(MSG, Change to Tool Dia = {toolC} ||| Total drills for tool T{tool} = {t_drills})
M0)PPT",
    R"PPT(
G00 X{x_toolchange} Y{x_toolchange} Z{z_move})PPT",
    R"PPT(
M5
G00 X{x_toolchange} Y{x_toolchange} Z{z_toolchange}
T{tool}
M6    
(MSG, Change to Tool Dia = {toolC})
M0)PPT",
    R"PPT(
G00 X{x_toolchange} Y{x_toolchange} Z{z_move})PPT",
};

}  // namespace flatcam::pp_templates
