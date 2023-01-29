# ShitCraft

**Как скомпилировать и поиграть в этот говно крафт?**

Легко и просто!

Пишем в консоль:

    git clone https://github.com/MrArtur4Ik/shitcraft
    cd shitcraft

Дальше:

## Linux (Makefile g++):

Перед компиляцией установите библиотеки SFML

Для Debian/Ubuntu/Mint:

    apt install libsfml-dev
Для Arch Linux:

    sudo pacman -Sy sfml

Компиляция:

    cmake -G "Unix Makefiles"
    make
    cd bin

И для запуска:

    ./shitcraft

Для тех кто не имеет библиотек:

    chmod +x start
    ./start

## Windows (Visual Studio):

    скоро...

