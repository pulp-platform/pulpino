all: ips pulpino pulpemu sw

# Create list of all *.dcp files based on the subdirectories of ips/
IP_OUTPUTS:=$(foreach d,$(subst ips/,,$(wildcard ips/*)),ips/$(d)/ip/$(d).dcp)
.PHONY: ips
ips: clk mem fp_fma

.PHONY: clk
clk: ips/xilinx_clock_manager/ip/xilinx_clock_manager.dcp

ips/xilinx_clock_manager/ip/xilinx_clock_manager.dcp:
	$(MAKE) -C ips/xilinx_clock_manager/ clean all

.PHONY: mem
mem: ips/xilinx_mem_8192x32/ip/xilinx_mem_8192x32.dcp

ips/xilinx_mem_8192x32/ip/xilinx_mem_8192x32.dcp:
	$(MAKE) -C ips/xilinx_mem_8192x32/ clean all

.PHONY: fp_fma
fp_fma: ips/xilinx_fp_fma/ip/xilinx_fp_fma.dcp

ips/xilinx_fp_fma/ip/xilinx_fp_fma.dcp:
	$(MAKE) -C ips/xilinx_fp_fma/ clean all


.PHONY: pulpino
pulpino: pulpino/pulpino.edn

pulpino/pulpino.edn: $(IP_OUTPUTS)
	$(MAKE) -C pulpino clean all


.PHONY: pulpemu
pulpemu: pulpemu/pulpemu_top.bit pulpemu/pulpemu_top.sysdef

# Only pattern rules handle multiple targets in a single recipe by
# invoking it only once cf. https://www.gnu.org/software/make/manual/html_node/Pattern-Intro.html
%emu/pulpemu_top.bit %emu/pulpemu_top.sysdef: %ino/pulpino.edn
	$(MAKE) -C pulpemu clean all


.PHONY: sw
sw: pulpemu/pulpemu_top.bit pulpemu/pulpemu_top.sysdef
	$(MAKE) -C sw all


.PHONY: clean
clean:
	$(MAKE) -C ips/xilinx_clock_manager/ clean
	$(MAKE) -C ips/xilinx_mem_8192x32/ clean
	$(MAKE) -C ips/xilinx_fp_fma/ clean
	$(MAKE) -C pulpino clean
	$(MAKE) -C pulpemu clean
	$(MAKE) -C sw clean
