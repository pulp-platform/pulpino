module cluster_clock_gating
(
    input  logic clk_i,
    input  logic en_i,
    input  logic test_en_i,
    output logic clk_o
);

    LAGCEPM12W  clk_gate_i
    (
        .GCK(clk_o),
        .CK(clk_i),
        .E(en_i),
        .SE(test_en_i)
    );

endmodule
