@ echo off
cd C:\Users\kzibs\Desktop\repos\chess_c\ProgramC\
gcc -c ChessProgram.c -o ChessProgram.o
gcc -shared -o ChessProgram.dll ChessProgram.o
pause