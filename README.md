# A mini Editing tool based on ffmpeg

## overview

In this code, I have implemented a mini GUI video editing tool in whitch you can customize the input file and choose the output directory.

## Prequisite

To run this code, you have to download ffmpeg, if you use a ubuntu machine, you can directly download with this command in the terminal

``` terminal
sudo apt install ffmpeg
```

and install sdl2 library

``` terminal
sudo apt install libsdl2-dev
```

## Compile

To compile, run the following code in your terminal

```terminal
gcc main.c -lSDL2  -o ffmpeg_gui_mini
```

## Execution

To run the executable, directly run the command in the terminal

```terminal
./ffmpeg_gui_mini
```
