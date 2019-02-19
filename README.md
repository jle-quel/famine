# Famine 

## Description

```
Famine

Usage:

./famine

```

## Installation

Must be run on a Linux distribution

```
make
./famine
```

## Description

This project is part of the Virology cursus at 42.

Famine is a simple virus, it doesn't

INTRODUCTION:

The idea behind Famine is to write a simple virus that is not about executing malicious payload but only about propagating itself in an organic/natural way.

_key point

Propagation:
Famine will infect different directory if you run it as root or as a simple user, but the way it chooses its target will be the same.

First, it will count the number of entries inside the predefined directories.
Now that we know how many executables are inside the directories, it will choose a random integer that is in range of the maximum entries and save one executable for each directory. Then select, again randomly,  one of those executables and start the infection routine.

This way every executable has as many chances to be chosen.

Infection:
The type of infection chosen is PT_NOTE to PT_LOAD. It is quite visible but give us the freedom to not care about the size of the parasite.

PT_NOTE is a segment that is created during the compilation, but before the linking. It contains innformation about OS specification information, and is actually not necessary for an executable to run (since the system will just assume the executable is native either way).

So that means that we can use PT_NOTE at our advantage and place our parasite inside.

Persistence:
Now that the file has been infected, I will use it to propagate itself on the whole system. To be able to do that I can just execute the infected file.
It will now do the whole routine of random selection again and execute it the infected until it finds a infected file (famine doesn't infect on an already infected file).

This way, I force the virus to propagate itself as fast as possible but with the chance factor, it may infect the whole system only by running famine one time, or it will let the user (if famine found and infected but not the whole system is infected) relaunch the routine.

## Author

Jefferson Le Quellec 🐜
