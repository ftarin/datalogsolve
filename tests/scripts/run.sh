#! /bin/bash

REPS=3

if test $# -eq 1
then
	REPS=$1
fi

rm -rf ../Results

echo "Running XSB"
cd time/timeXSB
./scriptTimeXSB $REPS
cd ../../

echo "Running Lua"
cd time/timeLua
./scriptTimeLua $REPS
cd ../../

echo "Runing Iris"
cd time/timeIris
./scriptTimeIris $REPS
cd ../../

echo "Running DataSolve V1"
cd time/timeDataSolve
./scriptTimeDataSolve $REPS
cd ../../

echo "Running DataSolve V2"
cd time/timeDataSolveV2
./scriptTimeDataSolve $REPS
cd ../../

# Crear grafica
echo "set encoding iso_8859_1;
set key right;
set xlabel 'Hechos'; set ylabel 'Segundos';
set title 'Resolutores Datalog';
set terminal postscript 'Helvetica' 18;
set output 'resultado.eps';
plot '../results/datalogsolve/results.time' w lp title 'DataSolve', '../results/datalogsolve/results-v2.time' w lp title 'DataSolve Optimizado', '../results/iris/results.time' w lp title 'Iris', '../results/xsb/results.time' w lp title 'Xsb', '../results/lua/results.time' w lp title 'Lua'" | gnuplot

evince resultado.eps &
