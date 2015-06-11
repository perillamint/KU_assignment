#!/bin/bash

for i in *.dat
do
  (echo dataname = \"${i%\.*}\"; cat gnuplot-command) | gnuplot
done
