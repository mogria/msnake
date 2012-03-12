# Mogria's Snake

A simple snake game written in C using the ncurses library.

![msnake Screenshot](https://raw.github.com/mogria/msnake/master/screenshot.png)

* * *

**Author:**  Mogria <m0gr14@gmail.com><br />
**Co-Author:**  Timo Furrer <timo.furrer@gmail.com><br />
**License:** MIT (see `LICENSE`)

* * *

# How to install

Simply enter the following command to compile the game:

    $ cmake .
    $ make

You'll need to have `cmake` installed. You'll also need the packages `build-essential`, `libncurses5`, `libncurses5-dev` if you are on a debian-based distro.
If you are _not_ a debian-based distro and you can't compile the game, look for similar packages in the repositories of your distribution. Execute the following command (with root privileges) to in install the packages on debian-based systems:

    # apt-get install cmake build-essential libncurses5 libncurses5-dev

After you've compiled the game you can execute the file `bin/msnake` to run the game.
If you want it to install this game to your system simply execute:

    # make install

Note: You need root-privileges to install this to your system

The install script simply moves the file `bin/msnake` to `/usr/bin/msnake`.
So, if your `PATH` is set correctly you can start the game using the command `msnake`

# How to create debian package of msnake

You can easily create a debian package of the msnake. If a binary does not exists the script will build one.
The following command is to create the debian package:

    $ ./makedeb.sh

And with the following command you can install it (You need root-privileges):

    # dpkg -i msnake.deb

# TODO

This list contains stuff we want to implement/change in the game.

* Better dialog system (a function to create common dialogs)
* A Highscore System
* A Settings Menu
* More food with diffrent effects?
* Better documentation of the source code
