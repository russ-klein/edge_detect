module top(
    input   clk,
    input   reset_bar
  );

  wire         aw_ready;
  wire         aw_valid;
  wire [43:0] aw_msg;
  wire         w_ready;
  wire         w_valid;
  wire [72:0]  w_msg;
  wire         b_ready;
  wire         b_valid;
  wire [5:0]   b_msg;
  wire         ar_ready;
  wire         ar_valid;
  wire [43:0]  ar_msg;
  wire         r_ready;
  wire         r_valid;
  wire [70:0]  r_msg;

  systemc_subsystem_wrapper scs_w (
     .clk       (clk),
     .reset_bar (reset_bar),

     .aw_ready_port       (aw_ready),
     .aw_valid_port       (aw_valid),
     .aw_msg_port         (aw_msg),
     .w_ready_port        (w_ready),
     .w_valid_port        (w_valid),
     .w_msg_port          (w_msg),
     .b_ready_port        (b_ready),
     .b_valid_port        (b_valid),
     .b_msg_port          (b_msg),
     .ar_ready_port       (ar_ready),
     .ar_valid_port       (ar_valid),
     .ar_msg_port         (ar_msg),
     .r_ready_port        (r_ready),
     .r_valid_port        (r_valid),
     .r_msg_port          (r_msg)
  );


  rocket_subsystem riscv(
     .clock       (clk),
     .reset       (!reset_bar),

     .periph_aw_ready     (aw_ready),
     .periph_aw_valid     (aw_valid),
     .periph_aw_msg       (aw_msg),
     .periph_w_ready      (w_ready),
     .periph_w_valid      (w_valid),
     .periph_w_msg        (w_msg),
     .periph_b_ready      (b_ready),
     .periph_b_valid      (b_valid),
     .periph_b_msg        (b_msg),
     .periph_ar_ready     (ar_ready),
     .periph_ar_valid     (ar_valid),
     .periph_ar_msg       (ar_msg),
     .periph_r_ready      (r_ready),
     .periph_r_valid      (r_valid),
     .periph_r_msg        (r_msg)
   );


endmodule
