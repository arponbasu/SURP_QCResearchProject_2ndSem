#!/bin/bash


PYTHON_INPUT_FILE=$1
TIMER_SWITCH=$2

clear
echo "Welcome to Intermediate Code Generation program"
echo "================================================"
echo "$(date '+%Y-%m-%d %H:%M:%S') ... Starting run with input file $PYTHON_INPUT_FILE"


PYTHON_SCRIPT_NAME="FullFactorized.py"

PREFIX="temp_"
CPP_INPUT_FILE="{$PREFIX}{$PYTHON_INPUT_FILE}"

echo "Input File Name : $PYTHON_INPUT_FILE"

echo "Command to be executed by this script now : python3 $PYTHON_SCRIPT_NAME $PYTHON_INPUT_FILE"
python3 $PYTHON_SCRIPT_NAME $PYTHON_INPUT_FILE

CPP_INPUT_FILE="$PREFIX$PYTHON_INPUT_FILE"
echo "CPP Input File : $CPP_INPUT_FILE"

echo "Generating C++ executable (Note1 :- This step may cause problems if you are not on a Linux machine; Note2 :- This may take a few seconds to execute)"
g++ -o ffg *.cpp *.h -std=c++11

echo "Running C++ code..."
./ffg $CPP_INPUT_FILE $TIMER_SWITCH


echo "Run Complete. Removing $CPP_INPUT_FILE ..."


rm -f $CPP_INPUT_FILE

echo "$(date '+%Y-%m-%d %H:%M:%S') ... Run Successful"
echo "==============================================="
