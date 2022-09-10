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

@R2
M=0  //R2=0

(LOOP)
    @R1
    D=M  // d=i-R1

    @END
    D;JEQ  // if R1 == 0 goto STOP

    @R1
    M=D-1   // R1=R1-1

    @R2
    D=M    //load R2

    @R0
    D=D+M  //newsum=R2+R0

    @R2
    M=D    // sum=newsum

    @LOOP
    0;JMP

(END)
    @END
    0;JMP