`define SPI_STD     2'b00
`define SPI_QUAD_TX 2'b01
`define SPI_QUAD_RX 2'b10

typedef struct packed {
    logic [31:0] addr;
    logic [31:0] data;
} spi_stim;

interface SPI ();
    logic       clk;
    logic [2:0] sdo;
    logic [2:0] sdi;
    logic       csn;
    logic [1:0] padmode;

    modport Slave (
        input clk, 
        output sdo, 
        input sdi, 
        input csn, 
        output padmode
    );
    modport Master (
        output clk, 
        output sdo, 
        input sdi, 
        output csn, 
        output padmode
    );
endinterface

class spi_master #(int semiperiode = 5000);
    /* SPI interface */
    local virtual SPI.Master spi_i;

    local logic use_quad = 0;

    function new (SPI s);
        this.spi_i = s;
    endfunction : new

    function void set_quad_spi(logic q);
        this.use_quad = q;
    endfunction

    function logic get_quad_spi(logic);
        return this.use_quad;
    endfunction

    task send_cmd(input logic [7:0] cmd, input logic [31:0] addr);
        this.send(cmd);
        this.send(addr);
    endtask : send_cmd

    task send_data(input logic [31:0] data);
        this.send(data);
    endtask : send_data

    task write_reg(input logic [7:0] cmd, input logic [7:0] reg_val);
        this.send(cmd);
        this.send(reg_value);
    endtask : wrtie_reg

    task read_word(input logic [7:0] cmd, input logic [31:0] addr, output logic [31:0] data);
        this.send_cmd(cmd, addr);
        this.receive(data);
    endtask : read_word

    task send_file(input string filename);
        spi_stim stimuli [];
        $readmemh(filename, stimuli);

        this.send_cmd(8'h2, stimuli.addr[0]);

        for (int i = 0; i < stimuli.size(); i++)
        begin
            this.send_data(stimuli.data[i]);
            if (stimuli.addr[i] != stimuli.addr[i + 1] - 32'h4)
            begin
                #semiperiode
                #semiperiode
                this.send_cmd(8'h2, stimuli.addr[i]);
            end     
        end

    endtask : send_file

    task spi_check(input string filename);
        
    endtask : spi_check
    
    /** Private Methods - SPI Logic **/
    local task send(input logic[] data);
        spi_i.csn = 1'b0;
        
        #semiperiode
        #semiperiode

        if (this.use_quad) 
        begin
            spi_i.padmode = SPI_QUAD_TX;
            for (int i = data.size()/4; i > 0; i--)
            begin
              for (int j = 3; j >= 0; j-- )
                spi_i.sdo[j] = data[4*i-j+1];

              this.clock(1);
            end
        end else 
        begin
            spi_i.padmode = SPI_STD;
            for (int i = data.size(); i > 0; i--)
            begin
                spi_i.sdo[0] = data[i];
                this.clock(1);
            end
        end
        spi_i.csn = 1'b1;
    endtask : send

    local task receive(input logic[] address, output logic[] data);
        spi_i.csn = 1'b0;

        #semiperiode
        #semiperiode
        
        if (this.use_quad) 
        begin
            spi_i.padmode = SPI_QUAD_RX;
            for (int i = data.size()/4; i > 0; i--)
            begin
              for (int j = 3; j >= 0; j-- )
                data[4*i-j+1] = spi_i.sdi[j];

              this.clock(1);
            end
        end else 
        begin
            spi_i.padmode = SPI_STD;
            for (int i = data.size(); i > 0; i--)
            begin
                data[i] = spi_i.sdi[0];
                this.clock(1);
            end
        end 
        spi_i.csn = 1'b1;
    endtask

    local task clock(input int cycles);
        for(int i = 0; i < cycles; i++)
        begin
            #semiperiode spi_i.sck = 1;
            #semiperiode spi_i.sck = 0;
        end
    endtask
endclass : spi_master