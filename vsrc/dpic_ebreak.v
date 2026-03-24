import "DPI-C" function void npc_ebreak(input int code);

module dpic_ebreak(
  input          clk,
  input   [31:0] inst,
  input   [31:0] pc
);

  // EBREAK 
  localparam EBREAK     = 32'h00100073;
  localparam EBREAK1    = 32'h00c00067; //default 程序的ebreak
  localparam EBREAK2    = 32'h21820213; //mem hex halt
  localparam EBREAK3    = 32'h22400213; //sum hex halt



  always @(posedge clk) 
  begin
    if (inst == EBREAK || inst == EBREAK1 || inst == EBREAK2 || inst == EBREAK3) begin
      // pass PC as code/context to the DPI-C handler
      npc_ebreak(pc);
    end
  end

endmodule

