`include "add_full.v"
module carry_transfer(a,b);

input[7:0] a;
output[7:0] b;

wire[7:0] b;
wire c0,c1,c2,c3,c4,c5,c6,c7;

add_full a1(a[0],1'b0,1'b1,b[0],c0);
add_full a2(a[1],1'b0,c0,b[1],c1);
add_full a3(a[2],1'b0,c1,b[2],c2);
add_full a4(a[3],1'b0,c2,b[3],c3);
add_full a5(a[4],1'b0,c3,b[4],c4);
add_full a6(a[5],1'b0,c4,b[5],c5);
add_full a7(a[6],1'b0,c5,b[6],c6);
add_full a8(a[7],1'b0,c6,b[7],c7);

endmodule
