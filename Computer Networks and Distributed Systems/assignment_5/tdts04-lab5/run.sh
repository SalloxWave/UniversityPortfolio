#!/bin/bash

xterm -hold -e make orbd &
xterm -hold -e make server &
xterm -hold -e make client