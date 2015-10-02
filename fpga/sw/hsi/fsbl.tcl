open_hw_design ./pulpemu_top.sysdef
generate_app -hw pulpemu_top -os standalone -proc ps7_cortexa9_0 -app zynq_fsbl -compile -sw fsbl -dir ./fsbl
exit
