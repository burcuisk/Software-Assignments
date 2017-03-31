module not_8bit (a,out);

input[7:0] a;
output[7:0] out;

wire[7:0] out;

assign out[0] = ~a[0];
assign out[1] = ~a[1];
assign out[2] = ~a[2];
assign out[3] = ~a[3];
assign out[4] = ~a[4];
assign out[5] = ~a[5];
assign out[6] = ~a[6];
assign out[7] = ~a[7];

endmodule
