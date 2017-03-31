.globl main

.data

str: .asciiz "hello"

.text

main: 
	la	 	$a0,str 					   # function argument $a0 = str address
	add     $v0,$0,$0					  
	jal		length						   # call length function load $v0 register length
	addi 	$s7,$s7,1					   # $s7 = palindrome = 1
	addi	$v0, $v0,-1					   # $v0 = length-1
	la		$t5,str 					   # $t5 = straddress
	add  	$t6,$t5,$v0					   # $t6 = last char address
	
loop: 
	slt	 	$t7,$t6,$t5					  
	bne		$t7,$0,return				   # last char address < first char address return
	lb	 	$t1, 0($t5)					   # $t1 =  str[first]
	lb	 	$t2, 0($t6)					   # $t2 = str[last]
	bne	 	$t2,$t1,returnZero			   # str[first] not equal str[last]  branch to returnZero
	addi 	$t5,$t5,1					   # first=first+1 (according to address)
	addi 	$t6,$t6,-1					   # last= last-1 (according to address)
	j loop
	
returnZero: 
	addi	$s7,$s7,-1					   # $s7 = 0 NOT polindorome
	li		$v0,10			    		   #exit
	syscall
	
return:
	li		$v0,10			    		   #exit
	syscall
	
length: 
	addi $t0,$t0,0						   # length = 0
	
for: 
	lb $t1, 0($a0)
	beqz $t1, exit						   # if $t1 = null branch to exit label
	addi $v0, $v0, 1					   # length = length+1
	addi $a0, $a0, 1					   # increment to address
	j for
	
exit: 
	jr $ra								   # break length function 