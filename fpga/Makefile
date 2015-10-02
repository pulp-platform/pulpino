all: ips pulpino pulpemu sw



.PHONY: ips
ips: clk mem mem_dp


.PHONY: clk
clk: ips/xilinx_clock_manager/ip/xilinx_clock_manager.dcp

ips/xilinx_clock_manager/ip/xilinx_clock_manager.dcp:
	make -C ips/xilinx_clock_manager/ clean all


.PHONY: mem
mem: ips/xilinx_mem_32768x32/ip/xilinx_mem_32768x32.dcp

ips/xilinx_mem_32768x32/ip/xilinx_mem_32768x32.dcp:
	make -C ips/xilinx_mem_32768x32/ clean all


.PHONY: mem_dp
mem_dp: ips/xilinx_mem_32768x32_dp/ip/xilinx_mem_32768x32_dp.dcp

ips/xilinx_mem_32768x32_dp/ip/xilinx_mem_32768x32_dp.dcp:
	make -C ips/xilinx_mem_32768x32_dp/ clean all



.PHONY: pulpino
pulpino: pulpino/pulpino.edf

pulpino/pulpino.edf:
	make -C pulpino clean all


.PHONY: pulpemu
pulpemu: pulpemu/pulpemu_top.bit

pulpemu/pulpemu_top.bit:
	make -C pulpemu clean all


.PHONY: sw
sw:
	make -C sw all
