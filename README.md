# Mogria's Snake

A simple snake game written in C using the ncurses library.

![msnake Screenshot](https://raw.github.com/mogria/msnake/master/screenshot.png)

* * *

**Author:**  Mogria <m0gr14@gmail.com><br />
**Co-Author:**  Timo Furrer <timo.furrer@gmail.com><br />
**License:** MIT (see `LICENSE`)

* * *

# How to install

## Cloning / Downloading

If you have git installed you can simply clone the repo using the following command.

    $ git clone https://github.com/mogria/msnake.git

The advantage of using git is you can update the downloaded code simply by entering:

    $ git pull

If you don't have git installed you can download a zip-file of the repository and extract it.

## Compiling

Change into the Git-Repository (or the folder where you extracted the zip file) and enter the following commands to compile the game:

    $ cmake .
    $ make

After you've compiled the game you can execute the file `bin/msnake` to run the game.

## Installing the Game to your System (only Linux & Mac)

To install this game to your system simply execute:

    # make install

Note: You need root-privileges to install this to your system

The install script simply moves the file `bin/msnake` to `/usr/bin/msnake`.
So, if your `PATH` is set correctly you can start the game using the command `msnake`


## Linux

To compile the game, you'll need to have `cmake` installed. You'll also need the packages `build-essential`, `libncurses5`, `libncurses5-dev` if you are on a debian-based distro.
If you are _not_ a debian-based distro and you can't compile the game, look for similar packages in the repositories of your distribution. Execute the following command (with root privileges) to in install the packages on debian-based systems:

    # apt-get install cmake build-essential libncurses5 libncurses5-dev


## Mac OS X

To compile the game, you'll need to have cMake.
To get cMake, go to [cMake.org](http;//www.cmake.org) and download and install the latest version. If the command `cmake` is a available in your Terminal you've installed cMake correctly.

## Windows

It was possible to compile the game on Windows with some minor changes to the source Code. You can download the compiled win32 executeable in the download section. The REpository will be updated soon to make it easier to compile the game on windows. Some instructions how to compile it with MinGW will also be added.


# How to create debian package of msnake

You can easily create a debian package of the msnake. If a binary does not exists the script will build one.
The following command is to create the debian package:

    $ ./makedeb.sh

And with the following command you can install it (You need root-privileges):

    # dpkg -i msnake.deb

# Contributing

To contribute to this awesome project, you can visit the repo on [GitHub](http://github.com/mogria/msnake) and fork it and create your own branch. You can pick a task from the TODO list below (or simply do something else). If you finished your work, create a pull request on GitHub and we'll probably merge your stuff to our repository.

# TODO

This list contains stuff we want to implement or change.

* Create a better dialog for entering the name
* create a simple display in the corner which displays the current score while playing
* don't create the highscore file at the current directory (in $HOME/.msnake/highscores.dat ?)
* add a scroll function to the highscore? or delete low entries?
* add a possibility to clear the highscore (function already written to clear the highscore, but called nowhere)
* don't add the time the game was paused to the gametime
* A Settings Menu
* More food with diffrent effects?
  * a fruit which changes the direction of the snake? (or a key?)
  * more eat-range for a short time?
* Better documentation of the source code
* Optimize Code
