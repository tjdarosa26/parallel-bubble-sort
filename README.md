# Parallel Bubble Sort 
This experiment was a graduation work for the discipline of parallel and distributed programming.

## What is this experiment?
This experiment consists of observing the time difference between the execution of a parallel sorting algorithm (using multiple threads) and a sequential one (one thread)

## How to reproduce this experiment?
First, you have to compile the code and will need GCC (compiler) installed. Once you have the compiler, you have to run the following command on your terminal:
```
gcc file_name.c -pthread -o file_name
```
for example:
```
gcc bubble.c -pthread -o bubble
```

Keep reading to know how to execute them.

## Which arguments can I pass to the programs?
For the sequential program, you must pass the number of arrays to order, and their size. For the parallel program, there is one more argument: the number of threads you want.

Now, for executing the programs (once compiled) you have to type in your terminal something like the following examples:

For the sequential one:
```
./bubble 16 256 
```

for the parallel one:
```
./parallel_bubble 16 256 8
```

## Which results are expected?
The output of the programs is their time spended ordering the arrays. For a big number of arrays with many elements, you should expect a better perfomance from the parallel program. Now, for a small number of arrays without many elements, you should expect he sequential one to be better.

