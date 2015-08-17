PROJECT:=xilinx_mem_32768x32_dp

all:
	vivado -mode batch -source tcl/run.tcl
	cp xilinx_mem_32768x32_dp.srcs/sources_1/ip/xilinx_mem_32768x32_dp/xilinx_mem_32768x32_dp_stub.v ./ip
	cp xilinx_mem_32768x32_dp.srcs/sources_1/ip/xilinx_mem_32768x32_dp/xilinx_mem_32768x32_dp.xci ./ip
	cp xilinx_mem_32768x32_dp.srcs/sources_1/ip/xilinx_mem_32768x32_dp/xilinx_mem_32768x32_dp.veo ./ip
	cp xilinx_mem_32768x32_dp.srcs/sources_1/ip/xilinx_mem_32768x32_dp/xilinx_mem_32768x32_dp.dcp ./ip

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

