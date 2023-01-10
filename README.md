# Сортировка большой ленты  
Параметры ленты (задержка считывания, записи и движения) можно изменить  
в файле params.txt. По умолчанию все параметры = 20 мс.  

## compile 
make

## run 
sort: ./sort.out "inputfile" "outputfile"  
generator: ./generator.out "inputfile" "number_count"  
checker: ./checker.out "outputfile"

## run tests
make run_tests

## clean
make clean
