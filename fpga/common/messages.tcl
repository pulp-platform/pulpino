# sets up Vivado messages in a more sensible way

set_msg_config -id {[Synth 8-3352]}         -new_severity "critical warning"
set_msg_config -id {[Synth 8-350]}          -new_severity "critical warning"
set_msg_config -id {[Synth 8-2490]}         -new_severity "warning"
set_msg_config -id {[Synth 8-2306]}         -new_severity "info"
set_msg_config -id {[Synth 8-3331]}         -new_severity "critical warning"
set_msg_config -id {[Synth 8-3332]}         -new_severity "info"
set_msg_config -id {[Synth 8-2715]}         -new_severity "error"
set_msg_config -id {[Opt 31-35]}            -new_severity "info"
set_msg_config -id {[Opt 31-32]}            -new_severity "info"
set_msg_config -id {[Shape Builder 18-119]} -new_severity "warning"
set_msg_config -id {[Filemgmt 20-742]}      -new_severity "error"

