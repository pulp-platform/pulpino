PROJECT:=xilinx_mem_32768x32

all:
	vivado -mode batch -source tcl/run.tcl
	mkdir -p ip
	cp ${PROJECT}.srcs/sources_1/ip/${PROJECT}/${PROJECT}_stub.v ./ip
	cp ${PROJECT}.srcs/sources_1/ip/${PROJECT}/${PROJECT}.xci ./ip
	cp ${PROJECT}.srcs/sources_1/ip/${PROJECT}/${PROJECT}.veo ./ip
	cp ${PROJECT}.srcs/sources_1/ip/${PROJECT}/${PROJECT}.dcp ./ip

gui:
	vivado -mode gui -source tcl/run.tcl &

clean:
	rm -rf ip/*
	mkdir -p ip
	rm -rf ${PROJECT}.xpr
	rm -rf ${PROJECT}.hw
	rm -rf ${PROJECT}.data
	rm -rf ${PROJECT}.runs
	rm -rf ${PROJECT}.srcs
	rm -rf ${PROJECT}.cache
	rm -rf component.xml
	rm -rf vivado*.jou
	rm -rf vivado*.log
	rm -rf vivado*.str
	rm -rf xgui

