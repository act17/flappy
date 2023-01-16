# flappy
A Flappy-Type game that I made.

# NOTE - ARCHIVED
  As of today, January 15th, 2023, I'm deciding to archive this respository. I do not intend to make any updates to this project, and as consequence of that, I believe it would be wise to archive the repository.

# Compiling
Unlike the initial release of my last project, *invreturn*, I actually have a functioning ``makefile`` for this project. Due to my lack of knowledge on the subject of makefiles, however, *the makefile is only compatable with GCC*. If you have GCC installed, simple run the command ``make`` to compile the program. You need the NCurses packages in order to compile and run, which are - correct me if I'm wrong - ``libncurses5`` and ``libncurses5-dev`` on the Ubuntu repository. If you're using a compiler other than GCC, then either edit the "gcc" in (the?) ``makefile`` with the compiler of your choice.
Another thing of note is that, to my knowledge, the program is *not* compatable with non-POSIX systems due to the usage of POSIX-threads. 

# Usage
Usage is very simple. Press any key to leave the main menu, and press any key to make Flappy - the little yellow square - move up. Try to get Flappy to go through the holes between the green pipes. If you fail, you'll be given a game over.

# Updates

*Version 1.0* (May 14th, 2022)
- Initial release.
