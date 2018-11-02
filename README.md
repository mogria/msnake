# Mogria's Snake
[![Build Status](https://travis-ci.org/mogria/msnake.svg)](https://travis-ci.org/mogria/msnake)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

A simple snake game written in C using the ncurses library.

![msnake screenshot](.github/screenshot.png)

---

**Author:**  Mogria <m0gr14@gmail.com>

**Co-Author:**  Timo Furrer <timo.furrer@gmail.com>

---

## Table of contents

* [Table of contents](#table-of-contents)
* [Installation instructions](#installation-instructions)
    * [Cloning or downloading the repository](#cloning-or-downloading-the-repository)
    * [Compiling](#compiling)
        * [Linux](#linux)
        * [Mac OS X](#mac-os-x)
        * [Windows (Cygwin)](#windows-cygwin)
        * [Windows (MinGW)](#windows-mingw)
    * [Installing the game to your system](#installing-the-game-to-your-system)
        * [Creating a Debian package](#creating-a-debian-package)
* [Contributing](#contributing)
* [To-do](#to-do)
* [Ideas for the future](#ideas-for-the-future)

## Installation instructions

### Cloning or downloading the repository

If you have git installed you can simply clone the repo using the following command:

```console
git clone https://github.com/mogria/msnake.git
```

The advantage of using git is that you can update the downloaded code simply by entering:

```console
git pull
```

If you don't have git installed you can download a zip file of the repository by clicking on the green
`Clone or download` button and then extract the downloaded zip file.

### Compiling

Change into the cloned git repository (or the folder where you extracted the zip file) and enter the following commands
to compile the game:

```console
mkdir build
cd build
cmake ..
make
```

After you've compiled the game you can execute the `bin/msnake` file to start the game.

#### Linux

To compile the game, you will need to have `cmake` installed. Additionally, you will need the `build-essential`,
`libncurses5` and `libncurses5-dev` packages if you are on a Debian-based distribution.

If yu are _not_ on a Debian-based distribution and you are unable to compile the game, look for similar packages in the
repositories of your distribution.

Execute the following command with root privileges to install the packages on a Debian-based distribution.

```console
apt-get install cmake build-essential libncurses5 libncurses5-dev
```

#### Mac OS X

To compile the game, you will need to have access to `cmake`. To get CMake, either install it via Homebrew using
`brew install cmake` or download the latest version from [cmake.org](http://www.cmake.org).

If the `cmake` command is available in your terminal you have successfully installed CMake.

#### Windows (Cygwin)

You need to have the packages `ncurses-devel` and `cmake` installed.

#### Windows (MinGW)

You can either download an old compiled win32 executable in the download section or compile the game yourself.
The latter requires you to have CMake and MinGW installed. Another prerequisite is the `pdcurses` packaged, installed
using `mingw-get`.

Finally, open the command line and run:

```console
cmake -g "MinGW Makefiles" .
make
```

### Installing the game to your system

To install this game to your system simply execute the following command with root-privileges:

```console
make install
```

The install script simply moves `bin/msnake` to `/usr/bin/msnake`. Assuming your `PATH` environment variable is set up
correctly, this will allow you to start the game by running `msnake` in your terminal.

#### Creating a Debian package

You can easily create a Debian package of msnake. If a binary does not yet exists the script will build one.
Simply run the following script to build the package:

```console
./makedeb.sh
```

And with the following command you can install it, provided you have root-privileges:

```console
dpkg -i msnake.deb
```

## Contributing

To contribute to this awesome project, you can visit [its repository on GitHub](http://github.com/mogria/msnake), fork
it and create your own branch.

You can pick a task from the to-do list below or simply do something else. Once you have finished your work, create a
pull request on GitHub and we will likely merge your stuff into our repository.

## To-do

This list contains a list of features we want to implement or change.

* Add a scroll function to the highscore or delete lower entries?
* A settings menu
* More food with different effects?
    * A fruit which changes the direction of the snake or its controls?
* Better documentation of the source code
* Optimize the code
    * The snake moves are inefficient, as every part is moved which makes the game very slow if you have a large snake.
      We should rather move/copy the last part to the new position
* Improve color coding

## Ideas for the future
* Make different difficulty mode with different speeds and amounts of fruits. Also provide a "free"/"classic" mode which
  is the current game mode.
* Add levels and quests?
