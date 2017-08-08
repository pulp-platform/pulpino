create_clock -period 50.000 -name clk      [get_nets {clk}]
create_clock -period 40.000 -name spi_sck  [get_nets {spi_clk_i}]
create_clock -period 40.000 -name tck      [get_nets {tck_i}]

# define false paths between all clocks
set_clock_groups -asynchronous \
                 -group { clk } \
                 -group { spi_sck } \
                 -group { tck }
