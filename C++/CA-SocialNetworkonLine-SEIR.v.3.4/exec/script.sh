#!/usr/bin/bash
###############################################################################
# by Marcelo Zamith - mzamith@ufrrj.br - April 1st, 2025
# Script used to execute the inforamtion propagation on server
# Command:
# ./script.sh 1000 1 6
###############################################################################


#Variables defined by console parameters
dir_home="/home/marcelo/Documents/02-Trabalhos/03.Develop/ACs/Information/sources/C++/CA-SocialNetworkonLine-SEIR.v.3.4/exec/" #the main server directory used to execute codes and instances
exec_code="IModelCA++.exec" #Executable file name
prefix=$(date +%Y%m%d-%H%M%S) #prefix used to identify unique each simulation and log
dir_out="$dir_home$prefix"
json_file="config.template.json" #Do not erase. It is the config template
new_json_file_name="config-$prefix.json"

#parameters from console
nodes=$1 #100
power=$2 #1
min_connections=$3 #6
echo "<Begin script>" >> log.txt
#echo "nodes: $nodes / power $power /$min_connections"
if [[ -z "$nodes" ]]; then
   echo "variable nodes is not defined" >> log.txt
   exit
fi
if [[ -z "$power" ]]; then
   echo "variable power is not defined" >> log.txt
   exit
fi
if [[ -z "$min_connections" ]]; then
   echo "variable min_connections is not defined" >> log.txt
   exit
fi

###############################################################################
###############################################################################
###############################################################################
###############################################################################
###############################################################################
# Begin script
if [ ! -d "$dir_out" ]; then
  mkdir $dir_out
fi

#call R
file_name="BA-N-$nodes-P-$power-C-$min_connections.net"
echo "File name is: $file_name" >> log.txt
echo "Call R" >> log.txt
if [ ! -f "$file_name" ]; then
    Rscript Modelo-BarabasiAlbert.R $nodes $power $min_connections >> log.txt
fi
if [ -f $file_name ]; then
    echo 'File was created' >> log.txt
else
    echo 'Error' >> log.txt
fi

#reset output file
echo '' > "$new_json_file_name"

#saving json file with new R .net file and simulation label
sed -e "s/filename.net/$file_name/g" -e "s/simulationname/$prefix/g" "$json_file" > "$new_json_file_name"
echo "$new_json_file_name was created" >> log.txt
echo "Compute the simulation" >> log.txt
$dir_home$exec_code $dir_home$new_json_file_name >> log.txt

#Moving csv files to directory so that we can check
mv "$dir_home/statistic.$prefix.csv"  $dir_out
if [ -f "$dir_home/agent.$prefix.ppm" ]; then
    mv "$dir_home/agent.$prefix.ppm"  $dir_out
fi
#mv "$dir_home*.ppm"  $dir_out
cp -f "$file_name"  $dir_out

echo "<End script>" >> log.txt
mv log.txt $dir_out


#This output must be shown in client app through php output API
echo "OUTPUT: $dir_out"
