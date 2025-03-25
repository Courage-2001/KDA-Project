/*
IMPORTANT:
    Since installing library, we must set up the enviroment on every new solution we create for a c++ project.
    Steps to take:
        1) Make sure cpp file exist in order to edit properties of solution
        2) right click on solution and go to properties
        3) make sure config -> all configs and platform -> all platforms at the top
        4) go to settings for C/C++, and add additional include directories. They are $(WXWIN)\include & $(WXWIN)\include\msvc
        5) expand linker settings and click on system. change subsystem -> windows subsystem and click apply
        6) set platform to Win32, goto general in linker and for additional lib dir, write $(WXWIN)\lib\vc_lib
        7) set platform to x64 and for additional lib dir, write $(WXWIN)\lib\vc_x64_lib and then click apply
*/

