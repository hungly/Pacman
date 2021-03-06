Pacman
======

Pacman

COSC2451

Programming Techniques

Copyright 2014 Ly Quoc Hung (s3426511), Ly Quan Liem (s3426110), Nguyen Vinh Linh (s3410595), Doan Hai Dang (s3425475)

RMIT International University Vietnam

This assignment includes software developed by the above mentioned students.

Assignment description:

  The classic Pacman game based on the one made by Namco in 1980 with level editor and individual AI for each of the
  4 ghosts. This program is the assignment for "Programming Techniques" course
  (COSC2451) at RMIT International University Vietnam.

2. Game features
  a. Players can choose the difficulty of game, the higher difficulty, the faster game is, and also, the more intelligent ghosts are.
  b. This game has high score broad which will save ten highest scores and player names. 
  c. Because of the difficulty of this game, we also offer cheat codes for gamers. There are two cheat codes, first is for god mode, second is for live cheat code:
    + god mode: UP UP DOWN DOWN LEFT RIGHT LEFT RIGHT b a
    + extra lifes: UP UP DOWN DOWN LEFT LEFT RIGHT RIGHT LEFT RIGHT
  
  d. During gameplay, you can pause game by pressing "ESC"
  e. After pausing game, you can resume game, save game or load game.

  NOTE:
    It seems that the program may display the map incorrectly sometimes. This program can simply fixed by quit game and restart. Pause game may even solve this program.

    Dang's ghost does not have proper memory allocation which may cause memory leak sometimes. It also seems to cause segmentation fault and crash the program.

    Linh's ghost seldom access unexist variable (using valgrind to check). However, this problem is not critical and does not crash the program

3. Map editor
  
  The program use a command line interface to display the current loaded map on the
  screen. The program ad two mode: edit mode and command mode:
  
  Edit mode:
  
    In this mode user can move around the map the create by default or by
    command and edit the current character at the cursor with:
    
      q/Q: add an upper left corner character
      
      e/E: add an upper right corner character
      
      z/Z: add a lower left corner character
      
      c/C: add a lower right corner character
      
      w/x: add a vertical bar character
      
      a/d: add a horizontal bar character
      
      s: add a small pellet character
      
      S: add a power pellet character
      
      f/F: add a fruit spawn point character
      
      g/G: add a ghost spawn point character
      
      p/P: add a pacman spawn point character
      
      ' ' (space): erase the current character
      
    Additionally, user can switch from edit mode to command mode by pressing
    ":" (colon) key.
    
    All the above command will take effect immediately after the respect key
    is pressed.
    
  Command mode:
  
    This mode provide user with an vi/vim like command mode to manipulate the map file. The command for this mode are:
      
      w [filename]: save the current loaded map with the file name specify in
      the [filename] field.
      
        Note:
          
          This command will check if the end of the [filename] is ended with
          ".pac" or not. If [filename] end with ".pac" then it will use this
          file name, otherwise ".pac" will be automatically added to the end
          of [filename].
          
          This command will overridden any file that already exist on the
          disk with the same file name after appending the ".pac".
          
      w: save the current loaded map with the file name specify in the "Map
      file name" field on the screen. Regarding the file name, this command
      will treat it the same way as the "w [filename]" command.
          
      q: quit the program without saving any change made.
      
      wq [filename]: quit the program saving all change in the file with the
      file name specify in the [filename] field. Regarding the file name, this
      command will treat it the same way as the "w [filename]" command.
      
      wq: quit the program saving all change in the file with the file name
      specify in the "Map file name" field on the screen. Regarding the file
      name, this command will treat it the same way as the "w [filename]"
      command.
      
      r [filename]: read a map in the "levels" directory with the name specify
      in the [filename]. This command will return an error if no such file with
      [filename] found in the directory. This command will read the file with
      the exact name provided with or without ".pac". E.g.: it will read the
      file "example" instead of "example.pac".
      
      n [filename] [row] [col]: create a new map with file name specify in
      [filename] and with size specify in the [row] and [col]. This command
      will return an error if not enough arguments are provided or there is an
      error in the argument. The size limit of the map will be dynamic and
      based on the current size of the program terminal.
      
      a [author(s)]: modify the author of the current loaded map. The author is
      displayed in the "Author" field on the screen and will saved in the map
      file. The author must had the format [name] <[email]>, there can be many
      authors but each author must have the above format and separate by a ','
      (comma).
      
      t [maptittle]: modify the tittle of the current loaded map. The tittle is
      displayed in the "Map tittle" field on the screen and will be saved in
      the map file.
      
      auto: automatically add small pellet character to all the possible place
      where a pacman can reach based on the pacman spawn point. This function
      only work if there is at least 1 pacman spawn point (p/P in edit mode)
      on the map and it will based on that spawn point. If there are more than
      one spawn points then the first one (if you read the map from left to 
      right and top to bottom) will be used.
      
    All the above command only work after hitting "Enter" button.
    
    User can exit command mode any time by pressing "Escape" key and it will
    work immediately.
    
  NOTE:
  
    If you maximize the program window (using the maximize window button) and
    then create a map that fit the size of the window (number of row larger
    that 22 and number of column larger than 64) and then restore the window
    to its original size (using minimize button) then the map will not
    display correctly. Its is recommended to not switch to maximize and
    minimize continuously, you should choose only one window size (maximize or
    normal) and stck with it.

    There is a case of where our program can read other people map and save it
    normally but other people while they CAN read map produced by our program
    but their program will crash (segmentation fault) while attempting to save
    the map although there is no change made. Their program can read and write
    other people map normally though.
    
    More detail of the problem:
    
      People can read map file produced by our program normally without any
      problem.
      
      If they try to write that file back to disk with the same or different
      name their program will crash giving "segmentation fault" error.
      
      Our program can read map file produced by others programs and it can save
      the file normally.
      
      If we open a map file that produced by others programs and that file is
      working correctly (no crash while saving), if we save that file without
      doing any modification to the file then the problem appear on the others
      programs. There is no difference in the file as we can tell while open by
      a text editor and while checking the file using a program that compare
      the original and newly saved file in binary mode (compare the bits).
      
      We suspect that our program had written some strange characters in the
      file but our write function only write character in the array, integer
      for map height and width and "\n" character so we have yet to find out 
      the cause of the problem.
      
Third party resources:

  NCURSES
  
    This software uses the NCURSES library package, which is under MIT License.
    This package can be redistributed free of charge in any forms.

  Some methods implemented in the program are found on:

  	http://stackoverflow.com/questions/4770985/something-like-startswithstr-a-str-b-in-c

  	http://stackoverflow.com/questions/744766/how-to-compare-ends-of-strings-in-c
  	
  	http://en.chys.info/2009/09/esdelay-ncurses/

    http://stackoverflow.com/questions/4204666/how-to-list-files-in-a-directory-in-a-c-program

    http://rosettacode.org/wiki/Dijkstra's_algorithm#C

    http://stackoverflow.com/questions/1910724/c-retrieving-total-line-numbers-in-a-file

    http://web.mit.edu/eranki/www/tutorials/search/
