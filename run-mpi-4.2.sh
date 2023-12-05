for p in 2, 3, 4, 6, 8
do
    printf "Executing with p = %s\n" $p
    mpic++ -o ./dist/mpi/task4.2 ./mpi/task4.2.cpp && mpirun -n $p ./dist/mpi/task4.2
    echo
done
