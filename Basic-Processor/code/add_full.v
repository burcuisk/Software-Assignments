module add_full(a,b,cin,s,cout);

input a,b,cin;
output s,cout;

wire s,cout;

assign s = a ^ b ^ cin;
assign cout = (a & cin) | (b & cin) | (a & b );

endmodule
