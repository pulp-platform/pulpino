PROJECT:=pulpemu

all: synth

synth:
	vivado -mode batch -source tcl/run.tcl
	cp pulpemu.runs/impl_1/pulpemu_top.bit pulpemu_top.bit

impl:
	rm -rf .Xil
	vivado -mode batch -source tcl/impl.tcl
	cp pulpemu_top.runs/impl_1/pulpemu_top.bit .
	cp pulpemu_top.runs/impl_1/*.dcp .

gui-synth:
	vivado -mode gui -source tcl/run.tcl &

gui-impl:
	vivado -mode gui -source tcl/impl.tcl &

continue:
	vivado -mode gui pulpemu_top.xpr

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
	rm -rf ${PROJECT}.hwdef
	rm -rf *.log
	rm -rf ulpsoc.txt
	rm -rf ulpcluster.txt
	rm -rf component.xml
	rm -rf vivado*
	rm -rf xgui
	rm -rf .Xil
	rm -rf ps_clock*
	rm -rf pulpemu_top*
	rm -rf pulpemu.edf

