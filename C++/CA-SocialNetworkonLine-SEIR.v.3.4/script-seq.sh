#!/usr/bin/bash
prefix=$(date +%Y%m%d-%H%M%S)
file_name="all your bases are belong to us!"
json_file="config.template.json" #Do not erase. It is the config template

sed -e "s/filename.net/$file_name/g" -e "s/simulationname/$prefix/g" "$json_file"
