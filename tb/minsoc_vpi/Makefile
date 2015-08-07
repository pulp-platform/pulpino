# If we are at ETH, use modelsim-10.2.c
ifneq (,$(findstring ee.ethz.ch, $(shell hostname)))
	MTI_HOME = /usr/pack/modelsim-10.2c-kgf/questasim
endif

ifneq (,$(findstring uhura, $(shell hostname)))
	MTI_HOME = /opt/cad/mentor/modelsim_10.0c1/modeltech
endif

ifneq (,$(findstring vader, $(shell hostname)))
	MTI_HOME = /opt/cad/mentor/modelsim_10.0c1/modeltech
endif

ifneq (,$(findstring winnfield, $(shell hostname)))
	MTI_HOME = /home/cad/mentor/questasim_10.1d/questasim
endif

ifneq (,$(findstring vega, $(shell hostname)))
	MTI_HOME = /home/cad/mentor/questasim_10.3d/questasim
endif

ifneq (,$(findstring micrel205, $(shell hostname)))
	MTI_HOME = /home/cad/mentor/questasim_10.1d/questasim
endif


ifneq (,$(findstring VaccaBoia, $(shell hostname)))
        MTI_HOME = /home/cad/mentor/questasim
endif


all:
	gcc -c -g -DMODELSIM_VPI -fPIC -I$(MTI_HOME)/include src/jp-io-vpi.c -o obj/jtag_vpi.o
	ld -shared -E -o lib/jtag_vpi.so obj/jtag_vpi.o
	cp lib/jtag_vpi.so ../../vsim/work/

