possible_blocks=("openmp")

printf "Enter block name ("
printf -v joined '%s' "${possible_blocks[@]}"
printf "${joined%,}"
printf '): '
read block

if [[ ! " ${possible_blocks[*]} " =~ " ${block} " ]]; then
    echo "No such block!"
    exit 1
fi

printf "Enter task number (01, 02 etc.): "
read num

mkdir -p ./dist/$block
g++ -o ./dist/$block/task$num -fopenmp ./$block/task$num.cpp && ./dist/$block/task$num
