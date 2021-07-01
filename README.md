# Mad Max

The project description is in the file `P88222_en.pdf`.

## Getting started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

1. Open a console and `cd` to the directory where you extracted the source code.

2. Run

    ```bash
    > make all
    ```

    to build the game and all the players. Note that `Makefile` identifies as a player any file matching `AI*.cc`.

3. This creates an executable file called `Game`. This executable allows you to run a game using a command like:

    ```bash
    > ./Game Bowsette Dummy Dummy Dummy -s 30 -i default.cnf -o default.res
    ```

    This starts a match, with random seed 30, of one instance of the player Bowsette and three instances of the player Dummy, in the board defined in `default.cnf`. The output of this match is redirected to `default.res`.

4. To watch a game, open the viewer file `viewer.html` with your browser and load the file `default.res`.
