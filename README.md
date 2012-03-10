# Mogria's Snake

A simple snake game written in C using the ncurses library.

***

**Author**:  Mogria <m0gr14@gmail.com>  
**Co-Author**:  Timo Furrer  
**License**: MIT (see `LICENSE`)

# How to install

Simply enter the following command to compile the game:

    $ cmake .
    $ make

After you've done that you can execute the file `bin/msnake`.
If you want it to install this game to you system simply execute:

    # make install

Note: You need root-privileges to install this to your system

The install script simply moves the file `bin/msnake` to `/usr/bin/msnake`.
So, if your `PATH` is set correctly you can start the game using the command `msnake`

# How to create debian package of msnake

You can easily create a debian package of the msnake. If a binary does not exists the script will build one.
The following command is to create the debian package:

    $ ./makedeb.sh

And with the following command you can install it:

    $ sudo dpkg -i msnake.deb

