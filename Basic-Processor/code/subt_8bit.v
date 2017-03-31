module subt_8bit (a,b,diff,borrow); 	// when subt values using adder.with for data2 ones complement

input[7:0] a,b;
output[7:0] diff;
output borrow;

wire [7:0] diff;
wire borrow;
wire c0,c1,c2,c3,c4,c5,c6,c7;

add_full a1(a[0],~b[0],1'b0,diff[0],c0);		
add_full a2(a[1],~b[1],c0,diff[1],c1);
add_full a3(a[2],~b[2],c1,diff[2],c2);
add_full a4(a[3],~b[3],c2,diff[3],c3);
add_full a5(a[4],~b[4],c3,diff[4],c4);
add_full a6(a[5],~b[5],c4,diff[5],c5);
add_full a7(a[6],~b[6],c5,diff[6],c6);
add_full a8(a[7],~b[7],c6,diff[7],borrow);

endmodule
