# Clic 3.0
Authoring tool for educational activities, precursor to [JClic](https://projectes.xtec.cat/clic)

![Clic 3.0](clic30.png)

## What is this?

"Clic" is an older authoring tool of educational activities created in 1992 for older Windows systems (3.1, 95, 98 and XP) by [Francesc Busquets](https://github.com/frncesc).

## How can we launch this software today?

To run "Músic" you will need to install a Windows 3.1-XP operating system emulator. We recommend using [Wine](https://www.winehq.org/), which is available for Linux and MacOS, or a virtual machine.

You will also need the executable files, which can be found in the [Releases](../../releases/) section of this repository. These files are packaged in different ZIP files, one for each available language, along with a collection of sample activities.

To run the program, unzip the ZIP file of your choice to any directory and run this command:

```bash
$ wine clic.exe
```

For more details on how the program works, see the documents in the "/docs" directory.

## History

"Músic" began to be designed in 1987. The first version, in Catalan, was published in the diskette magazine "[ESCUDELLA DE BITS](https://ddd.uab.cat/pub/quatec/quatec_a1989m1-2n20.pdf)", edited by MELIC (Moviments i Escoles per a la Informàtica a Catalunya) and GIRS (Grup d'Informàtica Rosa Sensat).

It was written entirely in [QuickBasic](https://en.wikipedia.org/wiki/QuickBASIC) and was intended to be an easy-to-use tool for integrating musical scores into [LOGO](https://en.wikipedia.org/wiki/Logo_(programming_language)) projects. Later, modules written in [Assembly language](https://en.wikipedia.org/wiki/Assembly_language) and [C](https://en.wikipedia.org/wiki/C_(programming_language)) were added. Importing and exporting files also required working with [Turbo Pascal](https://en.wikipedia.org/wiki/Turbo_Pascal), [Framework](https://en.wikipedia.org/wiki/Framework_(office_suite)) (FRED language), and the [Lisp](https://en.wikipedia.org/wiki/Lisp_(programming_language)) variant used by [Personal Composer](https://pcomposer.com/).

The program was edited by the [Programa d'Informàtica Educativa (PIE)](https://ca.wikipedia.org/wiki/Programa_d%27Inform%C3%A0tica_Educativa) of the Generalitat of Catalonia, and included in the CD-ROM "[Sinera en Disc](https://archive.org/details/1999-sinera)".

The Spanish version was awarded in the educational software competition convened by the Ministry of Education and Science of the Spanish government in 1990, and was also distributed to all schools in Spain by the [Programa de Nuevas Tecnologías de la Información y la Comunicación (PNTIC)](https://es.slideshare.net/slideshow/una-breve-historia-de-las-tic-educativas-en-espaa/75822170).

"Music" was also used in the [European Telematic Network for Education (RTEE)](http://www.xtec.cat/monografics/rtee/eng/rtee.htm)

## How to navigate the source code

All the source files are in `DEV/MUSIC`

- From the repository root, start the MS-DOS emulator [DOSBox](https://www.dosbox.com/)
```bash
$ dosbox
```
- In DOSBox, type:
```dosbatch
Z:\> MOUNT D .
```
This will mount the "D:" unit in the MS-DOS emulator, pointing to the current repository root.

- Go to `D:\DEV\MUSIC`
```dosbatch
Z:\> D:
D:\> CD DEV\MUSIC
```
Here you will the `.bat` and `.bas` files needed to edit, compile and convert the sources:

- `QB.BAT` starts QuickBasic with the necessary environment parameters.
- `DADES.BAS` reads the images (cover, help, keys...), text messages and other parameters, and includes them in the "MUSIC.BIN" file, necessary to run the program. In the LANG directory you will find different versions of DADES.BAS, along with their graphics, for various languages.
- `COMPILA.BAT` compiles a `.BAS` file, generating its binary equivalent `.OBJ`.
- `ASM.BAT` makes the same work with `.ASM` files.
- `FLB4.BAT` builds `MUSIC4.LIB` and `MUSIC4.QLB` from different `.OBJ` and `.BIN` files.
- `FES.BAT` builds the final executable file `MUSIC.EXE`
