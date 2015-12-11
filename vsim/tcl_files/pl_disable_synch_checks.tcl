proc disable_tchecks { path } {
  set instances [find instance -recursive $path]
  foreach element $instances {
    set splits [regexp -all -inline -- {\S+} $element]
    set instance [lindex $splits 0]
    catch {
        tcheck_set $instance OFF
        echo $instance
    }
  }
}

disable_tchecks /imperio_tb/top_i/pulpino_i/peripherals_i/axi_spi_slave_i/axi_spi_slave_i/u_syncro/*
disable_tchecks /imperio_tb/top_i/pulpino_i/peripherals_i/axi_spi_slave_i/axi_spi_slave_i/u_dcfifo_tx/u_din/full/*