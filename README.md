# Parallel Programming

## Ilyas Kalimullin, 11-101

### Instruction

#### Option 1:

Create a directory for compiled programs:
<br>`mkdir -p ./dist/openmp`<br>

To run tasks, execute:
<br>`g++ -o ./dist/openmp/task<N> -fopenmp ./openmp/task<N>.cpp && ./dist/openmp/task<N>`<br>
Don't forget to replace \<N\> with a number of task: 01, 02 etc.

Some tasks (e.g. task 01) require additional parameters (env):
<br>`g++ -o ./dist/openmp/task01 -fopenmp ./openmp/task01.cpp && export OMP_NUM_THREADS=8 && ./dist/openmp/task01`<br>

#### Option 2:

Simply run bash script:
<br>`./run.sh`<br>
or
<br>`bash ./run.sh`<br>

If you need extra env variables (e.g. task 1):
<br>`export OMP_NUM_THREADS=8 && bash ./run.sh`<br>
