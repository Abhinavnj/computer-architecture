read ax
read bx

mov ax cx
mul bx ax
mov ax cx
div cx bx
read dx
sub 5 dx
mul 2 dx
mov dx ax
jge 9 ax bx

add dx bx
mul dx ax
jle 14 bx ax

add 10 ax
sub ax cx
mul 2 ax
mov ax bx

jge 18 ax cx

add cx bx
div ax cx
mul cx dx

jl 25 0 bx

print ax
print bx
print cx
print dx
