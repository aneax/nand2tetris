// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
//
// This program only needs to handle arguments that satisfy
// R0 >= 0, R1 >= 0, and R0*R1 < 32768.

// Put your code here.


@R0
D=M;
@END
D;JLT

@R1
D=M
@END
D;JLT

@i
M=1

@sum
M=0

(LOOP)
    @i
    D=M  // load i

    @R1
    D=D-M  // d=i-R1

    @STOP
    D;JGT  // if i-R1 > 0 goto STOP

    @sum
    D=M    //load sum

    @R0
    D=D+M  //newsum=D+M

    @sum
    M=D    // sum=newsum

    @i
    M=M+1   // ++i

    @LOOP
    0;JMP

(STOP)
    @sum
    D=M

    @R2
    M=D  //RAM[2]=sum

(END)
    @END
    0;JMP