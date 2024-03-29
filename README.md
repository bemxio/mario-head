# Mario Head

A playful malware script designed to display a video of Mario's levitating head, humorously requesting access to your computer and subsequently crashing it with a Blue Screen of Death. The script is developed primarily using standard Windows libraries, particularly DirectShow.

## Building

To build the project, ensure you have the following dependencies installed:

- [MinGW](https://en.wikipedia.org/wiki/MinGW) - it's recommended to use [MSYS2](https://www.msys2.org/) on Windows. After installing MSYS2 (or if you're on an Arch-based Linux distro), execute `pacman -S mingw-w64-gcc` in the terminal to install MinGW. For other Linux distributions, search for `mingw-w64` in your package manager.

- [Git](https://git-scm.com/) - while optional, having Git installed simplifies the process of pulling the latest source code.

- `winpthreads` library from Git - only necessary for extending compatibility to Windows 2000/XP. Installation methods vary based on your OS:
    - For MSYS2, run `pacman -S mingw-w64-i686-winpthreads-git` in the terminal.
    - On an Arch-based Linux distro, you can use the [`mingw-w64-winpthreads-git`](https://aur.archlinux.org/packages/mingw-w64-winpthreads-git/) AUR package.
    - Other Linux distributions require manual building, with source code available [here](https://sourceforge.net/p/mingw-w64/mingw-w64).

Before building the project, verify the correctness of the [`Makefile`](Makefile), ensuring the [`CXX`](Makefile#L1) and [`WINDRES`](Makefile#L2) variables match your environment. For MSYS2 or Arch Linux, no changes are necessary. For other Linux distributions, adjust them accordingly.

Finally, execute `make` (or `mingw-w64-i686-make`) in the repository's root directory to build the executable.

## Video

The default video, [`assets/video.wmv`](assets/video.wmv), is sourced from [Mario Teaches Typing 2](https://www.mariowiki.com/Mario_Teaches_Typing_2). It features a clip of Mario's dialogue from the level select screen, extracted from [NintendoComplete's playthrough](https://youtu.be/PjyChE4NFXk?t=1370) at 22:50.

To use an alternative video, replace the `assets/video.wmv` file with your own Windows Media Video (WMV) file, ensuring it's named `video.wmv`. For video conversion, tools like [FFmpeg](https://ffmpeg.org/) can be employed:

```sh
ffmpeg -i input.mp4 -b 512k output.wmv
```

## License

This project is licensed under the MIT License. Refer to the [LICENSE](LICENSE) file for details.

Contributions, whether questions or pull requests, are highly encouraged and welcomed. Feel free to contribute in any capacity!
