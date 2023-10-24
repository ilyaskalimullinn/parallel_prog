possible_blocks=("openmp", "mpi")

printf "Enter block name ("
printf -v joined '%s' "${possible_blocks[@]}"
printf "${joined%,}"
printf '): '
read block

if [[ ! " ${possible_blocks[*]} " =~ "${block}" ]]; then
    echo "No such block!"
    exit 1
fi

printf "Enter task number (01, 02 etc.): "
read num

printf "Any params for run? "
read params

mkdir -p ./dist/$block

if [[ "$block" == "openmp" ]]; then
    g++ -o ./dist/$block/task$num -fopenmp ./$block/task$num.cpp && ./dist/$block/task$num $params
elif [[ "$block" == "mpi" ]]; then
    mpic++ -o ./dist/$block/task$num ./$block/task$num.cpp && mpirun $params ./dist/$block/task$num
fi
