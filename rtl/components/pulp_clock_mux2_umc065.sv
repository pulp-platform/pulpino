module pulp_clock_mux2
(
    input  logic clk0_i,
    input  logic clk1_i,
    input  logic clk_sel_i,
    output logic clk_o
  );

  CKMUX2M12W
  clk_mux_i
  (
    .A ( clk0_i    ),
    .B ( clk1_i    ),
    .S ( clk_sel_i ),
    .Z ( clk_o     )
  );

endmodule
