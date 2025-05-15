#!/usr/bin/bash
filename="all your bases are belong to us!"
while IFS= read -r line; do
    #echo "$line"
    echo "${line/filename.net/"$filename"}"

done < "config.template.json"
