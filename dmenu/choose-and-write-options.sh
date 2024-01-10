#!/bin/sh

options="$1"
# options="music\ncar\nverb\ngrass field\netc"

# -c     Centered
# -bw 5  Border width of 5 pixels
# -l 5   5 Rows, meaning only 1 column
# -i     case insensitive

selectedOption=$(echo $options | dmenu -c -bw 5 -l 5 -i)



xdotool keyup "enter"

xdotool type --delay 0 --clearmodifiers "$selectedOption"




