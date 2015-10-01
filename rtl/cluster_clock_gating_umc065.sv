// Regular VT
// LAGCEPM2R, LAGCEPM3R, LAGCEPM4R
// LAGCEPM6R, LAGCEPM8R
// LAGCEPM12R, LAGCEPM16R
// LAGCEPM20R

// Low VT
// LAGCEPM2W, LAGCEPM3W, LAGCEPM4W
// LAGCEPM6W, LAGCEPM8W
// LAGCEPM12W, LAGCEPM16W
// LAGCEPM20W

// High VT
// LAGCEPM2S, LAGCEPM3S, LAGCEPM4S
// LAGCEPM6S, LAGCEPM8S
// LAGCEPM12S, LAGCEPM16S
// LAGCEPM20S

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
