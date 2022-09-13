// push constant 17
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 3
@3
D=A
@SP
A=M
M=D
@SP
M=M+1
// sub
@SP
M=M-1 // SP--
A=M
D=M // D=*SP
@SP
A=M-1
M=M-D // M=*SP
(END)
@END
0;JMP
