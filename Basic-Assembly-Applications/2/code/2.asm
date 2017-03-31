.globl main

.data 

A: .word 2, 3, 4, 5, 6, 2, 3, 4, 5, 6
size :	 .word 10
K	 : 	 .word 2

.text 

main: 
	la		$t1, A
	lw		$s0,K				# store key K in $s0
	addi	$t2,$t2,0			# store 0 in $t2
	addi	$s1,$s1,0			# number of key in array $s1 = 0
	lw 		$s3,size			# s3 = size
	addi	$t5,$t5,4			# store 4 in $t5
	mul 	$t4,$t5,$s3			# t4 = sizex4 (total array size )
	j for

for: 
	beq $t2,$t4,return			# arraysize*4 = t2 return
	lw $t6,0($t1)				# $t6 = array[x]
	addi $t1,$t1,4				# address = address+4
	addi $t2,$t2,4				# $t2 = $t2+4
	beq $s0,$t6 increment		# if array[x]== key branch to increment
	j for
	
increment: 
	addi $s1,$s1,1				# numberofkeyinarray=numberofkeyinarray+1
	j for
	
return: 
	li	$v0,10					# exit
	syscall		
	
	  
	
	
