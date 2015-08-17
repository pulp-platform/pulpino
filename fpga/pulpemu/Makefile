PROJECT:=pulpemu

all: synth bitwrap

synth:
	vivado -mode batch -source tcl/run.tcl
	cp pulpemu.runs/impl_1/pulpemu_top.bit pulpemu_top.bit

impl:
	rm -rf .Xil
	vivado -mode batch -source tcl/impl.tcl
	cp pulpemu_top.runs/impl_1/pulpemu_top.bit .
	cp pulpemu_top.runs/impl_1/*.dcp .

gui:
	vivado -mode gui -source tcl/setup_impl.tcl &

gui-synth:
	vivado -mode gui -source tcl/run.tcl &

gui-impl:
	vivado -mode gui -source tcl/impl.tcl &

bitwrap:
	bootgen -image bitstream.bif -split bin -o i boot.bin
	rm -rf bitstream.bin u-boot.elf.bin
	mv pulpemu_top.bit.bin pulp4emu_latest.bit.bin

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

