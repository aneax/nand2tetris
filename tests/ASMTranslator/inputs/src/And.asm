// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1
// and
@SP
M=M-1 // SP--
A=M
D=M // D=*SP
@SP
A=M-1
M=D&M // M=*SP
// push constant 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1
// and
@SP
M=M-1 // SP--
A=M
D=M // D=*SP
@SP
A=M-1
M=D&M // M=*SP
// push constant 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
// and
@SP
M=M-1 // SP--
A=M
D=M // D=*SP
@SP
A=M-1
M=D&M // M=*SP
// push constant 2
@2
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
// and
@SP
M=M-1 // SP--
A=M
D=M // D=*SP
@SP
A=M-1
M=D&M // M=*SP
// push constant 2
@2
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 7
@7
D=A
@SP
A=M
M=D
@SP
M=M+1
// and
@SP
M=M-1 // SP--
A=M
D=M // D=*SP
@SP
A=M-1
M=D&M // M=*SP
(END)
@END
0;JMP
