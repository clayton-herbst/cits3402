#!/bin/bash
clear
mpirun --hostfile hostfile -np 2 ./sp -f ./examples/1024.in
echo "... completed p = 2"
mpirun --hostfile hostfile -np 4 ./sp -f ./examples/1024.in
echo "... completed p = 4"
mpirun --hostfile hostfile -np 6 ./sp -f ./examples/1024.in
echo "... completed p = 6"
mpirun --hostfile hostfile -np 8 ./sp -f ./examples/1024.in
echo "... completed p = 8"
mpirun --hostfile hostfile -np 10 ./sp -f ./examples/1024.in
echo "... completed p = 10"
mpirun --hostfile hostfile -np 12 ./sp -f ./examples/1024.in
echo "... completed p = 12"
mpirun --hostfile hostfile -np 14 ./sp -f ./examples/1024.in
echo "... completed p = 14"
echo "... script complete"
