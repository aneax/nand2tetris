// push constant 17
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 17
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
// eq
@SP
M=M-1 // SP--
A=M
D=M // D=*SP
@SP
M=M-1
A=M
D=M-D
@LL0True
D;JEQ
@SP
A=M
M=0 // false
@LL0Merge
0;JMP
(LL0True)
@SP
A=M
M=-1 // true
(LL0Merge)
@SP
M=M+1
// push constant 17
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 16
@16
D=A
@SP
A=M
M=D
@SP
M=M+1
// eq
@SP
M=M-1 // SP--
A=M
D=M // D=*SP
@SP
M=M-1
A=M
D=M-D
@LL1True
D;JEQ
@SP
A=M
M=0 // false
@LL1Merge
0;JMP
(LL1True)
@SP
A=M
M=-1 // true
(LL1Merge)
@SP
M=M+1
// push constant 16
@16
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 17
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
// eq
@SP
M=M-1 // SP--
A=M
D=M // D=*SP
@SP
M=M-1
A=M
D=M-D
@LL2True
D;JEQ
@SP
A=M
M=0 // false
@LL2Merge
0;JMP
(LL2True)
@SP
A=M
M=-1 // true
(LL2Merge)
@SP
M=M+1
// push constant 892
@892
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 891
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
// lt
@SP
M=M-1 // SP--
A=M
D=M // D=*SP
@SP
M=M-1
A=M
D=M-D
@LL3True
D;JLT
@SP
A=M
M=0 // false
@LL3Merge
0;JMP
(LL3True)
@SP
A=M
M=-1 // true
(LL3Merge)
@SP
M=M+1
// push constant 891
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 892
@892
D=A
@SP
A=M
M=D
@SP
M=M+1
// lt
@SP
M=M-1 // SP--
A=M
D=M // D=*SP
@SP
M=M-1
A=M
D=M-D
@LL4True
D;JLT
@SP
A=M
M=0 // false
@LL4Merge
0;JMP
(LL4True)
@SP
A=M
M=-1 // true
(LL4Merge)
@SP
M=M+1
// push constant 891
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 891
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
// lt
@SP
M=M-1 // SP--
A=M
D=M // D=*SP
@SP
M=M-1
A=M
D=M-D
@LL5True
D;JLT
@SP
A=M
M=0 // false
@LL5Merge
0;JMP
(LL5True)
@SP
A=M
M=-1 // true
(LL5Merge)
@SP
M=M+1
// push constant 32767
@32767
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 32766
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
// gt
@SP
M=M-1 // SP--
A=M
D=M // D=*SP
@SP
M=M-1
A=M
D=M-D
@LL6True
D;JGT
@SP
A=M
M=0 // false
@LL6Merge
0;JMP
(LL6True)
@SP
A=M
M=-1 // true
(LL6Merge)
@SP
M=M+1
// push constant 32766
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 32767
@32767
D=A
@SP
A=M
M=D
@SP
M=M+1
// gt
@SP
M=M-1 // SP--
A=M
D=M // D=*SP
@SP
M=M-1
A=M
D=M-D
@LL7True
D;JGT
@SP
A=M
M=0 // false
@LL7Merge
0;JMP
(LL7True)
@SP
A=M
M=-1 // true
(LL7Merge)
@SP
M=M+1
// push constant 32766
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 32766
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
// gt
@SP
M=M-1 // SP--
A=M
D=M // D=*SP
@SP
M=M-1
A=M
D=M-D
@LL8True
D;JGT
@SP
A=M
M=0 // false
@LL8Merge
0;JMP
(LL8True)
@SP
A=M
M=-1 // true
(LL8Merge)
@SP
M=M+1
// push constant 57
@57
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 31
@31
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 53
@53
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
// push constant 112
@112
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
// neg
@SP
A=M-1 // SP--
M=-M // M=*SP
// and
@SP
M=M-1 // SP--
A=M
D=M // D=*SP
@SP
A=M-1
M=D&M // M=*SP
// push constant 82
@82
D=A
@SP
A=M
M=D
@SP
M=M+1
// or
@SP
M=M-1 // SP--
A=M
D=M // D=*SP
@SP
A=M-1
M=D|M // M=*SP
// not
@SP
A=M-1 // SP--
M=!M // M=*SP
(END)
@END
0;JMP
