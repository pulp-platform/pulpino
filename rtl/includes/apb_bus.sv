// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.


`ifndef APB_BUS_SV
`define APB_BUS_SV

`include "config.sv"

// SOC PERIPHERALS APB BUS PARAMETRES
`define NB_MASTER  9

// MASTER PORT TO CVP
`define UART_START_ADDR       32'h1A10_0000
`define UART_END_ADDR         32'h1A10_0FFF

// MASTER PORT TO GPIO
`define GPIO_START_ADDR       32'h1A10_1000
`define GPIO_END_ADDR         32'h1A10_1FFF

// MASTER PORT TO SPI MASTER
`define SPI_START_ADDR        32'h1A10_2000
`define SPI_END_ADDR          32'h1A10_2FFF

// MASTER PORT TO TIMER
`define TIMER_START_ADDR      32'h1A10_3000
`define TIMER_END_ADDR        32'h1A10_3FFF

// MASTER PORT TO EVENT UNIT
`define EVENT_UNIT_START_ADDR 32'h1A10_4000
`define EVENT_UNIT_END_ADDR   32'h1A10_4FFF

// MASTER PORT TO I2C
`define I2C_START_ADDR        32'h1A10_5000
`define I2C_END_ADDR          32'h1A10_5FFF

// MASTER PORT TO FLL
`define FLL_START_ADDR        32'h1A10_6000
`define FLL_END_ADDR          32'h1A10_6FFF

// MASTER PORT TO SOC CTRL
`define SOC_CTRL_START_ADDR   32'h1A10_7000
`define SOC_CTRL_END_ADDR     32'h1A10_7FFF

// MASTER PORT TO DEBUG
`define DEBUG_START_ADDR      32'h1A11_0000
`define DEBUG_END_ADDR        32'h1A11_7FFF

`define APB_ASSIGN_SLAVE(lhs, rhs)     \
    assign lhs.paddr    = rhs.paddr;   \
    assign lhs.pwdata   = rhs.pwdata;  \
    assign lhs.pwrite   = rhs.pwrite;  \
    assign lhs.psel     = rhs.psel;    \
    assign lhs.penable  = rhs.penable; \
    assign rhs.prdata   = lhs.prdata;  \
    assign rhs.pready   = lhs.pready;  \
    assign rhs.pslverr  = lhs.pslverr;

`define APB_ASSIGN_MASTER(lhs, rhs) `APB_ASSIGN_SLAVE(rhs, lhs)

////////////////////////////////////////////////////////////////////////////////
//          Only general functions and definitions are defined here           //
//              These functions are not intended to be modified               //
////////////////////////////////////////////////////////////////////////////////

interface APB_BUS
#(
    parameter APB_ADDR_WIDTH = 32,
    parameter APB_DATA_WIDTH = 32
);

    logic [APB_ADDR_WIDTH-1:0]                                        paddr;
    logic [APB_DATA_WIDTH-1:0]                                        pwdata;
    logic                                                             pwrite;
    logic                                                             psel;
    logic                                                             penable;
    logic [APB_DATA_WIDTH-1:0]                                        prdata;
    logic                                                             pready;
    logic                                                             pslverr;


   // Master Side
   //***************************************
   modport Master
   (
      output      paddr,  pwdata,  pwrite, psel,  penable,
      input       prdata,          pready,        pslverr
   );

   // Slave Side
   //***************************************
   modport Slave
   (
      input      paddr,  pwdata,  pwrite, psel,  penable,
      output     prdata,          pready,        pslverr
   );

endinterface

`endif
