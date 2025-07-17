# `his`
A command history utility with icons and colors that works on Windows and GNU/Linux.

---

### GNU/Linux
![his command](https://github.com/terroo/terroo.github.io/blob/main/assets/img/commands/his.gif?raw=true) 


### Windows
![his command Windows](https://github.com/terroo/terroo.github.io/blob/main/assets/img/commands/his-win.gif?raw=true) 

---

## 📦 Dependencies

### 🪟 Windows
+ [Fonts](https://github.com/terroo/fonts)
+ [Git](https://terminalroot.com.br/2023/03/como-instalar-git-e-git-bash-no-windows.html)
+ [GCC](https://terminalroot.com/how-to-install-the-new-version-of-gcc-mingw-on-windows/) or [Clang](https://terminalroot.com/how-to-install-the-new-version-of-clang-llvm-on-windows/)
+ [PDCurses](https://terminalroot.com/how-to-install-pdcurses-ncurses-on-windows/)

### 🐂 GNU/Linux
+ [Fonts](https://github.com/terroo/fonts)
+ [Git](https://terminalroot.com/tags#git)
+ [GCC](https://terminalroot.com/tags#gcc) or [Clang](https://terminalroot.com/tags#clang)
+ [NCurses](https://terminalroot.com.br/ncurses)
+ [CMake](https://terminalroot.com/tags#cmake)

Example using APT:
```bash
sudo apt install build-essential cmake libncurses-dev git
```
> The fonts need to be installed manually as per the link above.

---

## 🚧 Build

### 🪟 Windows
> [PowerShell](https://terminalroot.com/customize-your-powershell-like-a-pro/)

```powershell
git clone https://github.com/terroo/his
Set-Location his
g++ -I C:\mingw64\include main.cpp his.cpp C:\mingw64\lib\pdcurses.a -o his
New-Item -Path "C:\His\bin" -ItemType Directory
Move-Item .\his.exe -Destination "C:\His\bin\"
```
> You can now exit the cloned directory and remove it.

Create an environment variable for your user
```powershell
[System.Environment]::SetEnvironmentVariable("Path", $env:Path + ";C:\His\bin", [System.EnvironmentVariableTarget]::User)
```
> Close and open the terminal and test: `his --version`


### 🐂 GNU/Linux
```powershell
git clone https://github.com/terroo/his
cd his
cmake . -B build
cmake --build build
sudo cmake --install build
```
> You can now exit the cloned directory and remove it: `cd .. && rm -rf his/`. And test: `his --version`

---

## 💼 Usage
> `his --help`

```bash
Usage:
his [options]

Options:
  --match-start,   -m  Match only the exact command.
  --no-show-icons, -n  Displays icons.
  --help,          -h  Show this message.
  --version,       -v  Show version info.
```

---

### 📹 Video tutorial showing step by step how the `his` command was made.
#### <https://youtu.be/his-command>

