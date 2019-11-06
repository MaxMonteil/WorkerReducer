# Worker Reducer

This is a simple implementation of the worker/reducer model implemented in C. It acceptes a number N of splits (< 11) to make on a large file (input.txt) and creates N worker processes to count the occurences of certain words (CMPS, CCE, ECE).

It then creates 3 reducers to sum the total occurences of these words accross all file splits.
