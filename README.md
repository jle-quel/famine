# Famine 

## Introduction

This project is part of the Virology cursus at 42.

The idea behind Famine is to write a simple virus that is not about executing malicious payload but only about propagating itself in an organic/natural way.

## Installation

Must be run on a Linux distribution

```
make
./famine
```

## Description

### Infection
The type of infection chosen is the `PT_NOTE to PT_LOAD`.
It is quite visible but gives us the freedom to not care about the size of the parasite.

`PT_NOTE` is a segment that contains information about OS specification and is actually not necessary at runtime. It means that we can use this segment at our advantage (since it already exist and we will not have to create one) and place our parasite inside.

`PT_NOTE` is a segment that is within the text segment, and its original size is really small `ex:0x2c4` and the parasite without optimization is more than 1 page `0x151B`. So we need to move this segment after the data segment, respecting the memory alignment.

`PT_NOTE` also must be converted into a `PT_LOAD` segment to be able to be loaded into memory, and have the same right as the text segment.
At this point, we can inject our parasite inside the segment.

### Propagation

Famine will infect different directory if you run it as root or as a simple user, but the way it chooses its target will be the same.

First, it will count the number of entries inside the predefined directories.
Now that we know how many executables are inside the directories, it will choose a random integer that is in range of the maximum entries and save one executable for each directory. Then select, again randomly,  one of those executables and start the infection routine. This way every executable has as many chances to be chosen.

Famine by default will try to infect as many executable as possible.

When a new binary is infected, the infector will execute it - which will start the whole (infection | propagation) routine. It means that famine has to infect only one executable on the system to be able to infect the whole system. Since the infected will do the work for me.

This execution flow will stop if the virus selects a binary that has already been infected.

### Logger

By default, famine is compiled in debug mode which means it will log on `tmp/trace`.

```
Famine: /tmp/test/ls has been infected
Famine: /tmp/test/sh has been infected
Famine: /tmp/test/mkdir has been infected
Famine: /tmp/test/bash has been infected
Famine: /tmp/test/touch has been infected
Famine: /tmp/test/sh can't open itself

Famine: /tmp/test/touch can't be infected

Famine: /tmp/test/echo has been infected
Famine: /tmp/test/touch can't be infected
```

## Author

Jefferson Le Quellec 🐜
