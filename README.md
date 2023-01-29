# ShitCraft

**Как скомпилировать и поиграть в этот говно крафт?**

Легко и просто!

Пишем в консоль:

    git clone https://github.com/MrArtur4Ik/shitcraft
    cd shitcraft
Перед компиляцией установите библиотеки SFML

Дальше:

## Linux (Makefile g++):

Для Debian/Ubuntu/Mint:

    apt install libsfml-dev
Для Arch Linux:

    sudo pacman -Sy sfml

Компиляция:

    cmake -G "Unix Makefiles"
    make
    cd bin

И для запуска:

    chmod +x main
    ./start

Для тех кто не имеет библиотек:

    chmod +x start
    ./start

## Windows (Visual Studio):

    скоро...

