// function SimpleFunction.test 2
(SimpleFunction.test)
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
// push local 0
@0
D=A
@LCL
A=D+M
D=M
@SP
A=M
M=D
@SP
M=M+1
// push local 1
@1
D=A
@LCL
A=D+M
D=M
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
// not
@SP
A=M-1 // SP--
M=!M // M=*SP
// push argument 0
@0
D=A
@ARG
A=D+M
D=M
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
// push argument 1
@1
D=A
@ARG
A=D+M
D=M
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
// return
@LCL
D=M
@R15
M=D
@R15
D=M
@5
A=D-A
D=M
@R14
M=D
@SP
A=M-1
D=M
@ARG
A=M
M=D
@ARG
D=M+1
@SP
M=D
@R15
D=M
@1
A=D-A
D=M
@THAT
M=D
@R15
D=M
@2
A=D-A
D=M
@THIS
M=D
@R15
D=M
@3
A=D-A
D=M
@ARG
M=D
@R15
D=M
@4
A=D-A
D=M
@LCL
M=D
@R14
A=M
0;JMP
(END)
@END
0;JMP
