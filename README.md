<div align="center">

# Catch me!

<img src="https://github.com/aniko33/Catch-me/assets/76649588/0c36e2f1-b4c0-4ec4-88be-c4299e3dc3a0" width=500px>

---

This malware will do a process injection on *smartscreen.exe* by injecting the `essential` DLL which will display a MessageBox saying *"TRY KILL ME"*. 
As soon as the **injected process is killed or after letting *25 seconds* pass it**
will **overwrite your MBR and then do a reboot**; all this with music playing in the background and the wallpaper changed 

</div>

## Compile
`make all`

## Add custom resources
song: `xxd -n song_wav -i <song_path>.wav > include/resource/song.c`

wallpaper (does not have to be jpg): `xxd -n wallpaper_jpg -i <wallpaper_path> > include/resource/wallpaper.c`

mbr: `nasm mbr.asm -o mbr.bin && xxd -n mbr_asm -i mbr.bin > include/resource/mbr.c`
