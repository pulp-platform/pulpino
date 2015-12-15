module cluster_clock_inverter
  (
    input  logic clk_i,
    output logic clk_o
   );

  CKINVM22WA
  clk_inv_i
  (
    .A ( clk_i ),
    .Z ( clk_o )
  );

endmodule
