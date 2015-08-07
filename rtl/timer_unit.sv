////////////////////////////////////////////////////////////////////////////////
// Company:        Multitherman Laboratory @ DEIS - University of Bologna     //
//                    Viale Risorgimento 2 40136                              //
//                    Bologna - fax 0512093785 -                              //
//                                                                            //
// Engineer:       Davide Rossi - davide.rossi@unibo.it                       //
//                                                                            //
// Additional contributions by:                                               //
//                  Igor Loi - igor.loi@unibo.it                              //
//                                                                            //
//                                                                            //
// Create Date:    13/02/2013                                                 // 
// Design Name:    ULPSoC                                                     // 
// Module Name:    ulpcluster_top                                             //
// Project Name:   ULPSoC                                                     //
// Language:       SystemVerilog                                              //
//                                                                            //
// Description:    ULPSoC cluster                                             //
//                                                                            //
//                                                                            //
// Revision:                                                                  //
// Revision v0.1 - File Created                                               //
// Revision v0.2 - Added 4 extra timers and events                            //
// Revision v0.3 - Added PER_ID_WIDTH parameters  (20/02/2015)                //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

module timer_unit
#(
  parameter   PER_ID_WIDTH = 5
  )
  (
    input  logic            clk_i,
    input  logic            rst_ni,

    input  logic            PSEL,
    input  logic            PENABLE,
    input  logic            PWRITE,
    input  logic [11:0]      PADDR,
    input  logic [31:0]     PWDATA,
    output logic [31:0]     PRDATA,
    output logic            PREADY,
    output logic            PSLVERR,

    output logic            event_t0_o,
    output logic            event_t1_o,
    output logic            event_t2_o,
    output logic            event_t3_o
    );

    enum logic [1:0] { TRANS_IDLE, TRANS_RES_HI, TRANS_RES_LO, TRANS_RES_WRITE } CS, NS;
    enum logic [1:0] { TIMER_IDLE, TIMER_RUN } CS_TIMER, NS_TIMER;


    logic                 s_start_counter, s_stop_counter, s_reset_counter, s_counter_running;
    logic [63:0]          s_count;

    logic                 s_start_counter_t0, s_stop_counter_t0, s_reset_counter_t0, s_counter_running_t0;
    logic                 s_start_counter_t1, s_stop_counter_t1, s_reset_counter_t1, s_counter_running_t1;
    logic                 s_start_counter_t2, s_stop_counter_t2, s_reset_counter_t2, s_counter_running_t2;
    logic                 s_start_counter_t3, s_stop_counter_t3, s_reset_counter_t3, s_counter_running_t3;
    logic [31:0]          s_count_t0;
    logic [31:0]          s_count_t1;
    logic [31:0]          s_count_t2;
    logic [31:0]          s_count_t3;
    logic [31:0]          r_count_t0_target;
    logic [31:0]          r_count_t1_target;
    logic [31:0]          r_count_t2_target;
    logic [31:0]          r_count_t3_target;
    logic [31:0]          s_count_t0_target_next;
    logic [31:0]          s_count_t1_target_next;
    logic [31:0]          s_count_t2_target_next;
    logic [31:0]          s_count_t3_target_next;

    logic  [2:0]          s_reg_sel_next;
    logic  [2:0]          r_reg_sel;

    //**********************************************************
    //*************** ADDRESS DECODER **************************
    //**********************************************************

    // UPDATE THE STATE
    always_ff @(posedge clk_i, negedge  rst_ni)
    begin : UPDATE_STATE
      if(rst_ni == 1'b0)
        CS <= TRANS_IDLE;
      else
        CS <= NS;
    end

    always_comb
    begin : COMPUTE_STATE
      s_start_counter         =   1'b0;
      s_stop_counter          =   1'b0;
      s_reset_counter         =   1'b0;
      s_start_counter_t0      =   1'b0;
      s_stop_counter_t0       =   1'b0;
      s_reset_counter_t0      =   1'b0;
      s_start_counter_t1      =   1'b0;
      s_stop_counter_t1       =   1'b0;
      s_reset_counter_t1      =   1'b0;
      s_start_counter_t2      =   1'b0;
      s_stop_counter_t2       =   1'b0;
      s_reset_counter_t2      =   1'b0;
      s_start_counter_t3      =   1'b0;
      s_stop_counter_t3       =   1'b0;
      s_reset_counter_t3      =   1'b0;
      s_count_t0_target_next  =   r_count_t0_target;
      s_count_t1_target_next  =   r_count_t1_target;
      s_count_t2_target_next  =   r_count_t2_target;
      s_count_t3_target_next  =   r_count_t3_target;
      PRDATA                  =   '0;
      PREADY                  =   1'b0;
      NS                      =   TRANS_IDLE;
      s_reg_sel_next          =   3'h0;

      begin
        case(CS)

          TRANS_IDLE:
          begin
            if ( PSEL && PENABLE )
            begin
              case(PADDR[9:2])

                8'h0:
                begin
                  s_start_counter = 1'b1;
                  NS = TRANS_RES_WRITE;
                end

                8'h1:
                begin
                  s_stop_counter = 1'b1;
                  NS = TRANS_RES_WRITE;
                end

                8'h2:
                begin
                  s_reset_counter = 1'b1;
                  NS = TRANS_RES_WRITE;
                end

                8'h3:
                begin
                  s_reg_sel_next = 3'h0;
                  NS = TRANS_RES_LO;
                end

                8'h4:
                begin
                  s_reg_sel_next = 3'h0;
                  NS = TRANS_RES_HI;
                end

                8'h10:
                begin
                  s_start_counter_t0 = 1'b1;
                  NS = TRANS_RES_WRITE;
                end

                8'h11:
                begin
                  s_stop_counter_t0 = 1'b1;
                  NS = TRANS_RES_WRITE;
                end

                8'h12:
                begin
                  s_reset_counter_t0 = 1'b1;
                  NS = TRANS_RES_WRITE;
                end

                8'h13:
                begin
                  if (PWRITE)
                    s_count_t0_target_next= PWDATA;
                  s_reg_sel_next = 3'h1;
                  NS = TRANS_RES_LO;
                end

                8'h20:
                begin
                  s_start_counter_t1 = 1'b1;
                  NS = TRANS_RES_WRITE;
                end

                8'h21:
                begin
                  s_stop_counter_t1 = 1'b1;
                  NS = TRANS_RES_WRITE;
                end

                8'h22:
                begin
                  s_reset_counter_t1 = 1'b1;
                  NS = TRANS_RES_WRITE;
                end

                8'h23:
                begin
                  if (PWRITE)
                    s_count_t1_target_next= PWDATA;
                  s_reg_sel_next = 3'h2;
                  NS = TRANS_RES_LO;
                end

                8'h30:
                begin
                  s_start_counter_t2 = 1'b1;
                  NS = TRANS_RES_WRITE;
                end

                8'h31:
                begin
                  s_stop_counter_t2 = 1'b1;
                  NS = TRANS_RES_WRITE;
                end

                8'h32:
                begin
                  s_reset_counter_t2 = 1'b1;
                  NS = TRANS_RES_WRITE;
                end

                8'h33:
                begin
                  if (PWRITE)
                    s_count_t2_target_next= PWDATA;
                  s_reg_sel_next = 3'h3;
                  NS = TRANS_RES_LO;
                end

                8'h40:
                begin
                  s_start_counter_t3 = 1'b1;
                  NS = TRANS_RES_WRITE;
                end

                8'h41:
                begin
                  s_stop_counter_t3 = 1'b1;
                  NS = TRANS_RES_WRITE;
                end

                8'h42:
                begin
                  s_reset_counter_t3 = 1'b1;
                  NS = TRANS_RES_WRITE;
                end

                8'h43:
                begin
                  if (PWRITE)
                    s_count_t3_target_next= PWDATA;
                  s_reg_sel_next = 3'h4;
                  NS = TRANS_RES_LO;
                end

                default:
                begin
                  s_start_counter         =   1'b0;
                  s_stop_counter          =   1'b0;
                  s_reset_counter         =   1'b0;
                  s_start_counter_t0      =   1'b0;
                  s_stop_counter_t0       =   1'b0;
                  s_reset_counter_t0      =   1'b0;
                  s_start_counter_t1      =   1'b0;
                  s_stop_counter_t1       =   1'b0;
                  s_reset_counter_t1      =   1'b0;
                  s_start_counter_t2      =   1'b0;
                  s_stop_counter_t2       =   1'b0;
                  s_reset_counter_t2      =   1'b0;
                  s_start_counter_t3      =   1'b0;
                  s_stop_counter_t3       =   1'b0;
                  s_reset_counter_t3      =   1'b0;
                  s_count_t0_target_next  =   r_count_t0_target;
                  s_count_t1_target_next  =   r_count_t1_target;
                  s_count_t2_target_next  =   r_count_t2_target;
                  s_count_t3_target_next  =   r_count_t3_target;
                  PRDATA                  =   '0;
                  PREADY                  =   1'b1;
                end

              endcase
            end
          end

          TRANS_RES_LO:
          begin
            case(r_reg_sel)
              0:
              PRDATA                = s_count[31:0];
              1:
              PRDATA                = s_count_t0[31:0];
              2:
              PRDATA                = s_count_t1[31:0];
              3:
              PRDATA                = s_count_t2[31:0];
              4:
              PRDATA                = s_count_t3[31:0];
              default:
              PRDATA                = s_count[31:0];
            endcase
            PREADY                = 1'b1;
            NS                    = TRANS_IDLE;
          end

          TRANS_RES_HI:
          begin
            PRDATA                = s_count[63:32];
            PREADY                = 1'b1;
            NS                    = TRANS_IDLE;
          end

          TRANS_RES_WRITE:
          begin
            PREADY                = 1'b1;
            NS                    = TRANS_IDLE;
          end

          default:
          begin
            NS                    = TRANS_IDLE;
          end

        endcase

      end

    end

    // UPDATE THE REG SEL
    always_ff @(posedge clk_i, negedge  rst_ni)
    begin : UPDATE_REG_SEL
      if(rst_ni == 1'b0)
      begin
        r_reg_sel         <=  3'h0;
        r_count_t0_target <= 32'h10000000;
        r_count_t1_target <= 32'h10000000;
        r_count_t2_target <= 32'h10000000;
        r_count_t3_target <= 32'h10000000;
      end
      else
      begin
        r_reg_sel         <= s_reg_sel_next;
        r_count_t0_target <= s_count_t0_target_next;
        r_count_t1_target <= s_count_t1_target_next;
        r_count_t2_target <= s_count_t2_target_next;
        r_count_t3_target <= s_count_t3_target_next;
      end
    end



    //**********************************************************
    //*************** TIMER ************************************
    //**********************************************************

    always_ff @(posedge clk_i, negedge rst_ni)
    begin : UPDATE_TIME
      if(rst_ni == 1'b0)
        CS_TIMER <= TIMER_IDLE;
      else
        CS_TIMER <= NS_TIMER;
    end

    always_comb
    begin : COMPUTE_TIME

      s_counter_running = 1'b0;

      case(CS_TIMER)

        TIMER_IDLE:
        begin
          s_counter_running = 1'b0;
          if ( s_start_counter == 1'b1 )
          begin
            NS_TIMER = TIMER_RUN;
          end
          else
          begin
            NS_TIMER = TIMER_IDLE;
          end
        end

        TIMER_RUN:
        begin
          s_counter_running = 1'b1;
          if ( s_stop_counter == 1'b1 )
          begin
            NS_TIMER = TIMER_IDLE;
          end
          else
          begin
            NS_TIMER = TIMER_RUN;
          end
        end

        default:
        begin
          NS_TIMER = TIMER_IDLE;
        end

      endcase

    end

    always_ff @(posedge clk_i, negedge rst_ni)
    begin
      if( rst_ni == 1'b0 )
      begin
        s_count <= 64'b0;
      end
      else
      begin
        if ( s_reset_counter == 1'b1 )
        begin
          s_count <= 64'b0;
        end
        else
          if ( s_counter_running == 1'b1 )
          begin
            s_count <= s_count +1;
          end 
        end
      end


      //**********************************************************
      //*************** CORE TIMERS ************************************
      //**********************************************************

      always_ff @(posedge clk_i, negedge rst_ni)
      begin 
        if(rst_ni == 1'b0)
        begin
          s_counter_running_t0 = 1'b0;
          s_counter_running_t1 = 1'b0;
          s_counter_running_t2 = 1'b0;
          s_counter_running_t3 = 1'b0;
        end
        else
        begin
          if ( s_start_counter_t0 == 1'b1 )
            s_counter_running_t0 = 1'b1;
          else if ( s_stop_counter_t0 == 1'b1 )
            s_counter_running_t0 = 1'b0;
          if ( s_start_counter_t1 == 1'b1 )
            s_counter_running_t1 = 1'b1;
          else if ( s_stop_counter_t1 == 1'b1 )
            s_counter_running_t1 = 1'b0;
          if ( s_start_counter_t2 == 1'b1 )
            s_counter_running_t2 = 1'b1;
          else if ( s_stop_counter_t2 == 1'b1 )
            s_counter_running_t2 = 1'b0;
          if ( s_start_counter_t3 == 1'b1 )
            s_counter_running_t3 = 1'b1;
          else if ( s_stop_counter_t3 == 1'b1 )
            s_counter_running_t3 = 1'b0;
        end
      end

      always_ff @(posedge clk_i, negedge rst_ni)
      begin
        if( rst_ni == 1'b0 )
        begin
          s_count_t0 <= 32'b0;
          s_count_t1 <= 32'b0;
          s_count_t2 <= 32'b0;
          s_count_t3 <= 32'b0;
        end
        else
        begin
          if (( s_reset_counter_t0 == 1'b1 ) || (s_count_t0 == r_count_t0_target)) 
            s_count_t0 <= 32'b0;
          else if ( s_counter_running_t0 == 1'b1 )
            s_count_t0 <= s_count_t0 +1;
          if (( s_reset_counter_t1 == 1'b1 ) || (s_count_t1 == r_count_t1_target)) 
            s_count_t1 <= 32'b0;
          else if ( s_counter_running_t1 == 1'b1 )
            s_count_t1 <= s_count_t1 +1;
          if (( s_reset_counter_t2 == 1'b1 ) || (s_count_t2 == r_count_t2_target)) 
            s_count_t2 <= 32'b0;
          else if ( s_counter_running_t2 == 1'b1 )
            s_count_t2 <= s_count_t2 +1;
          if (( s_reset_counter_t3 == 1'b1 ) || (s_count_t3 == r_count_t3_target)) 
            s_count_t3 <= 32'b0;
          else if ( s_counter_running_t3 == 1'b1 )
            s_count_t3 <= s_count_t3 +1;
        end
      end

      //**********************************************************
      //*************** BINDING OF OUTPUT SIGNALS  ***************
      //**********************************************************
      assign event_t0_o = (s_count_t0 == r_count_t0_target);
      assign event_t1_o = (s_count_t1 == r_count_t1_target);
      assign event_t2_o = (s_count_t2 == r_count_t2_target);
      assign event_t3_o = (s_count_t3 == r_count_t3_target);

    endmodule
