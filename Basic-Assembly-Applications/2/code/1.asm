.globl main


.text
main: 
	addi 	$s0,4						# load n value to $s0
	move	$a0, $s0 					# store n to function argument $a0
	jal		factorial					# call factorial

	move	$s1,$v0						#store output to $s1 
	li		$v0,10						#exit
	syscall

factorial:
	add		$t0,$a0,$0					# equalize $t0 to n
	add		$t2,$0,1					# equalize $t2 to 1
	add		$t3,$t0,$0					# equalize $t3 to n
	beq		$t0,$0,returnOne			# if n($t0) equal 0 branch returnOne label
	beq 	$t0,$t2,returnOne			# if n($t0) equal 1 branch returnOne label
	
for : 	
	beq		$t0,$t2, return				# if $t0 equal 1 branch to return label
	addi	$t1,$t0,-1				    # $t0 minus 1 equalize to $t1
	mul		$t3,$t3,$t1					# multiply t1 and $t3(first loop equal n) and equalize $t3
	addi	$t0,$t0,-1					# subtract 1 from $t0 
	j	for
	
return: 
	move	$v0,$t3						# load return value
	jr	$ra								# jump to parent call

returnOne: 
	move	$v0,$t2						# equalize return value to 1
	jr	$ra								# jump to parent call