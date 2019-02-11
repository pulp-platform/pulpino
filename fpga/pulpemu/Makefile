PROJECT:=pulpemu

all: synth

synth:
	vivado -mode batch -source tcl/run.tcl
	cp ${PROJECT}.runs/impl_1/${PROJECT}_top.bit .
	cp ${PROJECT}.runs/impl_1/${PROJECT}_top.sysdef .

impl:
	rm -rf .Xil
	vivado -mode batch -source tcl/impl.tcl
	cp pulpemu_top.runs/impl_1/pulpemu_top.bit .
	cp pulpemu.runs/impl_1/pulpemu_top.sysdef .
	cp pulpemu_top.runs/impl_1/*.dcp .

gui-synth:
	vivado -mode gui -source tcl/run.tcl &

gui-impl:
	vivado -mode gui -source tcl/impl.tcl &

continue:
	vivado -mode gui ${PROJECT}.xpr

clean-impl:
	rm -rf pulpemu_top*

clean:
	rm -rf ${PROJECT}.hw
	rm -rf ${PROJECT}.xpr
	rm -rf ${PROJECT}.data
	rm -rf ${PROJECT}.runs
	rm -rf ${PROJECT}.srcs
	rm -rf ${PROJECT}.sim
	rm -rf ${PROJECT}.sdk
	rm -rf ${PROJECT}.cache
	rm -rf ${PROJECT}.ioplanning
	rm -rf ${PROJECT}.ip_user_files
	rm -rf ${PROJECT}.hwdef
	rm -rf *.log
	rm -rf ulpsoc.txt
	rm -rf ulpcluster.txt
	rm -rf component.xml
	rm -rf vivado*
	rm -rf xgui
	rm -rf .Xil
	rm -rf ps_clock*
	rm -rf ${PROJECT}_top*
	rm -rf ${PROJECT}.edn

