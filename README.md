# SJF Approximation

This project processes a list of processes fetched from a .csv file and "simulates" the algorithm called "SJF Approximation".

- Processes are of type "tipoProcesso" and allocated into a fixed array "processo" of that type.
- This type is a struct that has important data about said processes, whom have time of processing required, time of arrival, etc (named accordingly in portuguese).
- The struct is also organized using a simple bubble sort algorithm, for the sake of being able to see better (on Round-Robin I utilized it more because I simply checked if there was a process that was arriving above the time variable).
- All processes are printed on cmd when they are being "processed", and also calculated their 'average waiting time' and 'turnaround time' at the end.

Moreover on the code itself, where I commented (on portuguese) what each block of code does.

----

Bonus: the file 'main round-robin.c' has the Round-Robin algorithm, which I accidentally programmed thinking that was the thing I was actually doing.
  On the Round-Robin program, q = 1.
