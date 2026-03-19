/*DPIC*/
import "DPI-C" function void pmem_write_u32(input int waddr, input int wdata);
import "DPI-C" function void pmem_write_u8(input int waddr, input byte wdata);

module LSU(

    input               clk,

    input               mem_re,
    input               mem_we,
    input       [1:0]   mem_width,
    input               mem_unsigned,
    input       [31:0]  wdata,

    input       [31:0]  addr,

    output reg  [31:0]  rdata

);

always@(posedge clk)
begin
    if(mem_we)
    begin
        case(mem_width)
            2'b00: begin // SB: 按地址选择需要写入的字节
                case (addr[1:0])
                    2'b00: pmem_write_u8(addr, wdata[7:0]);
                    2'b01: pmem_write_u8(addr, wdata[15:8]);
                    2'b10: pmem_write_u8(addr, wdata[23:16]);
                    2'b11: pmem_write_u8(addr, wdata[31:24]);
                endcase
            end
            2'b10: pmem_write_u32(addr, wdata); //sw四字节
            default: ;
        endcase

    end
end




endmodule

