// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.

// GET MEMORY RANGE FOR SCREEN


// Keyboard
@KBD
D=A
@LOOPEND
M=D

// Screen
@SCREEN
D=A
@ADDR
M=D  // ADDR = 16384


// This is the last memory unit for screen.
// Loop counter end
@LOOPEND
M=M-D

// Loop counter
@i
M=0  // i = 0

// Value that will fill the screen
// Initially zero
@FILLVALUE
M=0

// Start Looping
(LOOP)

    // Check Loop Counter
    @i
    D=M
    @LOOPEND
    D=D-M
    // If loop counter is larger than loopend, reset loopcounter
    // screen address
    @RESET
    D;JGE // RESET address to SCREEN

    // Read Keyboard
    @KBD
    D=M

    // If not equal to zero, fill=-1, else fill=0
    @FILL
    D;JNE

    @FILLVALUE
    M=0

(CONTINUE)
    
    // load fill value
    @FILLVALUE
    D=M
    
    // load address
    @ADDR
    A=M

    // update value
    M=D  // RAM[ADDR]=-1

    // update loop counter
    @i
    M=M+1 // i = i+1

    // Update address
    @1
    D=A
    @ADDR
    M=D+M // ADDR=ADDR+1
    @LOOP
    0;JMP


(FILL)
    @FILLVALUE
    M=-1

    @CONTINUE
    0;JMP


(RESET)
    // Reset position on screen
    @SCREEN
    D=A
    @ADDR
    M=D  // ADDR = 16384

    // Reset loopcounter
    @i
    M=0

    // start looping
    @LOOP
    0;JMP