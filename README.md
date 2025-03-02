# C Shell Code
this c code present here contains code of the working shell

## Assumptions
1. Assumed the max size of any arguments is having a max value of 4039
2. max number of backgroud process possible are also 4039
3. the shell will have the home directory as the working directory of the shell program which mean the path is relative to the working directory but when the path is given out of the working directory then it will show the absolute path

the shell code have the following functions like
### hop 
* like cd command in linux
* structure is hop < path/./../-/~>

### reveal 
* like the ls command in linux
* structure is reveal < flag> < path/./..>

### log 
* this is a history function this have the function like 
1. log - shows the history
2. log execute n- execute the nth lastest command
3. log purge - clear the history
* structure is log / log execute n / log purge
### proclore 
* this will print the detail of the process with given process id
* structure is proclore < pid >
* path absolute

### seek 
* search for the given file in the target directory
* structure is seek < flags > < filename > < target directory>

the shell will also execute some of the other commands of linux shell command as execvp system call is used
shell will also execute the background process which are having & symbol at the end of the command


## Assumptions for part - B
* commands like echo will work in the format echo < word > for example if we enter echo hello world, it will print hello world
*  my fg command will work only for the process created by my shell
* i had used alias keyword in the spec 9
* 