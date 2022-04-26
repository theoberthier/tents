#!/bin/bash 

for i in ../../../*.c
do
	ln -s $i ./
done

for j in ../../../*.h
do
	ln -s $j ./
done
