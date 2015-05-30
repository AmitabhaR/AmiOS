echo ------------------- AmiOS Build System --------------------

set "compiler_path=C:\MSVC\BIN\cl.exe"
set "linker_path=C:\MSVC\BIN\link.exe"
set "compiler_dir=C:\MSVC\BIN"
set "asm_path="C:\Program Files (x86)\MASM\BIN\ml""
set "asm_dir="C:\Program Files (x86)\MASM\BIN""
set "nasm_path="C:\Users\Riju\AppData\Local\nasm\nasm""
set "bochs_path="C:\Program Files (x86)\Bochs-2.6.7\bochs.exe""
set "compiler_args=/AT /Gs /Gx /c /Zl /G2 /DOS_COMPILE"

REM Lets Assemble the Assembler Programs
%asm_path% /AT /c %cd%\src\KernelEntry.asm

%nasm_path% -f bin %cd%\src\Boot.asm -o %cd%\Build\BootLoader.bin
%nasm_path% -f bin %cd%\src\Shell.asm -o %cd%\Build\Shellii.bin

REM Lets Compile the C Programs
%compiler_path% %compiler_args% %cd%\src\Kernel.c
%compiler_path% %compiler_args% %cd%\src\Display.c
%compiler_path% %compiler_args% %cd%\src\Disk.c
%compiler_path% %compiler_args% %cd%\src\String.c
%compiler_path% %compiler_args% %cd%\src\System.c
%compiler_path% %compiler_args% %cd%\src\Memory.c
%compiler_path% %compiler_args% %cd%\src\Process.c

REM Call Linker.
%linker_path% /MAP /T /NOD KernelEntry.obj Kernel.obj Display.obj Disk.obj String.obj System.obj Memory.obj Process.obj
copy /b KernelEntry.com %cd%\Build\Kernel.com

REM Make our floppy image.
FATX -b %cd%\Build\BootLoader.bin -k %cd%\Build\Kernel.com -o %cd%\Build\AmiOS.img %cd%\Build\Shellii.bin

REM Add the magic bytes with the file.

REM Delete Object Files.
del %compiler_dir%\*.obj
del *.obj
del *.com

%bochs_path% -f %cd%\Build\boschsrc.bxrc

echo -------------------- Build Complete ------------------