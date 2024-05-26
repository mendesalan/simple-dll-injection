### How to hook into a proccess using .dll

Firstly compile the hook.cpp file into a dll using GCC:
g++ -shared -o hook.dll hook.cpp


Secondly compile the main program which is the injector of the dll into the target proccess:
g++ injector.cpp -o injector.exe

Huge thanks to Kyle for his amazing [tutorial](https://kylehalladay.com/blog/2020/05/20/Hooking-Input-Snake-In-Notepad.html).