set out_dir [lindex $argv 0]
if { ${out_dir} eq "" } {
   puts "ERROR: Please supply an output directory!"
   exit 1
}

open_hw_design pulpemu_top.sysdef
set_repo_path ../device_tree_repo
create_sw_design device-tree -os device_tree -proc ps7_cortexa9_0
generate_target -dir ${out_dir}
