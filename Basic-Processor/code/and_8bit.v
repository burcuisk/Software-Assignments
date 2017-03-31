module and_8bit (a,b,out);

input[7:0] a,b;
output[7:0] out;

wire[7:0] out;

and(out[0],a[0],b[0]);
and(out[1],a[1],b[1]);
and(out[2],a[2],b[2]);
and(out[3],a[3],b[3]);
and(out[4],a[4],b[4]);
and(out[5],a[5],b[5]);
and(out[6],a[6],b[6]);
and(out[7],a[7],b[7]);

endmodule
