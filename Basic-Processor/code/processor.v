`include "adder_8bit.v"
`include "subt_8bit.v"
`include "carry_transfer.v"
`include "and_8bit.v"
`include "or_8bit.v"
`include "xor_8bit.v"
`include "not_8bit.v"

module Processor ( DATA1, DATA2, OPCODE, DATAOUT, FLAGS );

input[7:0] DATA1, DATA2;
input[3:0] OPCODE;
output[4:0] FLAGS;
output[7:0] DATAOUT;

reg[4:0] FLAGS;
reg[7:0] DATAOUT;
wire CF,SF;
wire [7:0] DATA1,DATA2;
wire [7:0] AddOut,SubtOut,SubtOutCarry,AndOut,OrOut,XorOut,NotOut;

adder_8bit p1(DATA1[7:0],DATA2[7:0],AddOut[7:0],CF); 
subt_8bit p2(DATA1[7:0],DATA2[7:0],SubtOut[7:0],SF);
carry_transfer p3(SubtOut[7:0],SubtOutCarry[7:0] );	// subtoutcarry = subtout + 1 carry bit transfer
and_8bit p4(DATA1[7:0],DATA2[7:0],AndOut[7:0]);
or_8bit p5(DATA1[7:0],DATA2[7:0],OrOut[7:0]);
xor_8bit p6(DATA1[7:0],DATA2[7:0],XorOut[7:0]);
not_8bit p7(DATA1[7:0],NotOut[7:0]);

always@(*)

begin

case(OPCODE)

4'b0000 : begin 
	  DATAOUT = AddOut;
	  if(CF == 1 ) 
		FLAGS = 5'b00100;		// set carry flag
	  else
		FLAGS = 5'b00000; 
	  end

4'b0001 : begin 				//SUB COMMAND
	  if ( SubtOutCarry == 0 ) 		// data1 = data2
	  	FLAGS = 5'b00000;
	 
	  else if(SF == 1) begin		// data1-data2 > 0
		DATAOUT = SubtOutCarry;		//SUBout + carrybit
		FLAGS = 5'b00000; end

          else begin
		DATAOUT = ~SubtOut;		// data1-data2 < 0 result sub convert to ones complement
		FLAGS = 5'b00010; end	
	  end

4'b0010 : begin					// and commands	
	  DATAOUT = AndOut;
	  FLAGS = 5'b00000;
	  end

4'b0011 : begin 				// or commands
	  DATAOUT = OrOut;
	  FLAGS = 5'b00000;
	  end	

4'b0100 : begin					// xor commands
	  DATAOUT = XorOut;
	  FLAGS = 5'b00000;
	  end	

4'b0101	: begin					//not commands
	  DATAOUT = NotOut;    
	  FLAGS = 5'b00000;
	  end

4'b0110 : begin					// CMP Commands
	  if( SubtOutCarry == 0 ) begin 	 // data1 = data2 situation
	  	FLAGS = 5'bx0001;	 	 //set zf
		DATAOUT = SubtOutCarry; end 	 
	 
	  else if( SF == 1 )  		   	 // data1 > data2 situation 
		FLAGS = 5'b10000; 	   	 // set COF

	  else 				         // data1 < data2 situation
	  	FLAGS = 5'b00000; 	 
	  
	  end

4'b0111: begin					// shift commands
	 DATAOUT = DATA1 << DATA2;  
	 FLAGS = 5'b00000;
	 end

4'b1000: begin
	 DATAOUT = DATA1 >> DATA2;  
	 FLAGS = 5'b00000;
	 end

default : DATAOUT = 8'b0;

endcase
end

endmodule





