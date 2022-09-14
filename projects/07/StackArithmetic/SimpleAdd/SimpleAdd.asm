// push constant 7
@7
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 8
@8
D=A
@SP
A=M
M=D
@SP
M=M+1
// add
@SP
M=M-1 // SP--
A=M
D=M // D=*SP
@SP
A=M-1
M=D+M // M=*SP
(END)
@END
0;JMP
