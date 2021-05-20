# UNIX-Bash

## About:

Implementation of a UNIX Bash using linux system calls with support for a few simple commands. 

## Supported Commands:

`listDir()` - ls

`showCurrentDir()` - pwd

`makeDir()` - mkdir

`changeDir()` - cd

`copyFile()` - cp

`moveFile()` - mv

`deleteFile()` - rm

`displayFile()` - cat


## Usage:

Download all included files, install make, run make command 

File Mode:

run ./pseudo-shell -f `<input file>`

`<input file>` is a file (supplied by you) filled with commands separated by ";". The program will execute all valid commands in the file before exiting.

Interactive Mode:

run ./pseudo-shell

The program will prompt with ">>>" enter any supported command followed by relevant parameters. 

