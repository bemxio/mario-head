# Mario Head
A playful malware script designed to display a video of Mario's levitating head, humorously requesting access to your computer and subsequently crashing it with a [Blue Screen of Death](https://en.wikipedia.org/wiki/Blue_screen_of_death). Developed by using standard Windows libraries, mainly [DirectShow](https://en.wikipedia.org/wiki/DirectShow).

## Building
To build the project, ensure you have the following dependencies installed:
- [MinGW](https://en.wikipedia.org/wiki/MinGW) with MSVCRT - on versions 12 and above, MinGW uses UCRT as the default C runtime, which causes compatibility issues with systems older than Windows 10.
    - If you're on Windows, it's recommended to use [MSYS2](https://www.msys2.org/). Simply run the installer, open the MSYS2 terminal, and execute `pacman -S mingw-w64-i686-toolchain` to install everything you need.
    - If you're on an Arch-based Linux distribution, install the [`mingw-w64-gcc`](https://archlinux.org/packages/extra/x86_64/mingw-w64-gcc) package, then build and install the [`mingw-w64-crt-msvcrt`](https://aur.archlinux.org/packages/mingw-w64-crt-msvcrt) and [`mingw-w64-headers-msvcrt`](https://aur.archlinux.org/packages/mingw-w64-headers-msvcrt) packages from the AUR.
    - For other Linux distros, follow the instructions on the [MinGW-w64 wiki](https://www.mingw-w64.org/downloads) or compile it from source. Make sure to install the MSVCRT variant, in case you want to run the executable on Windows 8.1 or earlier.
- [Make](https://en.wikipedia.org/wiki/Make_(software)) - included inside the MinGW toolchain package group on MSYS2; on other Linux distributions, you can install it via your package manager.
- `winpthreads` from Git - only necessary for extending compatibility to Windows XP.
    - On MSYS2, it is also included inside the MinGW toolchain, so no additional steps are needed.
    - On an Arch-based distro, you can use the [`mingw-w64-winpthreads-git`](https://aur.archlinux.org/packages/mingw-w64-winpthreads-git) AUR package.
    - Other Linux distributions require manual building, with source code available [here](https://sourceforge.net/p/mingw-w64/mingw-w64).

Before building the project, make sure the [`PREFIX`](Makefile#L2), [`CXX`](Makefile#L4) and [`WINDRES`](Makefile#L8) variables match your environment. For MSYS2 or Arch Linux, no changes are necessary. For other Linux distributions, adjust them accordingly.

Finally, execute `make` (or `mingw-w64-i686-make` if on MSYS2) in the repository's root directory to build the executable.

## Video
The default video, [`assets/video.wmv`](assets/video.wmv), is sourced from [Mario Teaches Typing 2](https://www.mariowiki.com/Mario_Teaches_Typing_2). It's a clip of Mario's dialogue from the level select screen, extracted from [NintendoComplete's playthrough](https://youtu.be/PjyChE4NFXk?t=1370) at 22:50.

To use an alternative video, replace the `assets/video.wmv` file with your own Windows Media Video (WMV) file, ensuring it's named `video.wmv`. For video conversion, you can use tools like [FFmpeg](https://ffmpeg.org/):

```sh
ffmpeg -i input.mp4 -b 512k output.wmv
```

## License
This project is licensed under the MIT License. Refer to the [LICENSE](LICENSE) file for details.

Contributions, whether issues or pull requests, are highly encouraged and welcomed. Feel free to contribute in any capacity!
