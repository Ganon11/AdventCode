# IntCode

This is an interpreter for [Advent of Code](https://adventofcode.com/) 2019's IntCode.

IntCode was first introduced in [Day 2](https://adventofcode.com/2019/day/2). This first iteration required **ADD**, **MULTIPLY**, and **HALT** commands.

IntCode was expanded in [Day 5](https://adventofcode.com/2019/day/5). In this iteration, the concept of *parameter modes* was introduced, and six more instructions were added: **INPUT**, **OUTPUT**, **JUMP-IF-TRUE**, **JUMP-IF-FALSE**, **LESS-THAN**, and **EQUALS**.

In [Day 7](https://adventofcode.com/2019/day/7), I changed how **INPUT** and **OUTPUT** worked by enqueueing input and appending output to a list. This was needed to implement message passing between multiple intcode programs running in parallel. I also added the **step** function to execute an intcode program one step at a time.