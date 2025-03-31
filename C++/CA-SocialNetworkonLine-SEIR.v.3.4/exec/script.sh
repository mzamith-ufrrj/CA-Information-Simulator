#!/usr/bin/bash
#Variables defined by console parameters
dir_home="/home/marcelo/Documents/02-Trabalhos/03.Develop/ACs/Information/sources/C++/CA-SocialNetworkonLine-SEIR.v.3.4/exec/" #the main server directory used to execute codes and instances
exec_code="IModelCA++.exec" #Executable file name
prefix=$(date +%Y%m%d-%H%M%S) #prefix used to identify unique each simulation and log
dir_out="$dir_home$prefix"
json_file="config.template.json" #Do not erase. It is the config template
new_json_file_name="config-$prefix.json"

#parameters from console
nodes=100
power=1
min_connections=6

# Begin script
if [ ! -d "$dir_out" ]; then
  mkdir $dir_out
fi

#call R
file_name="BA-N-$nodes-P-$power-C-$min_connections.net"
echo "File name is: $file_name"
echo "Call R"
if [ ! -f "$file_name" ]; then
    Rscript Modelo-BarabasiAlbert.R $nodes $power $min_connections
fi
if [ -f $file_name ]; then
    echo 'File was created'
else
    echo 'Error'
fi

#reset output file
echo '' > "$new_json_file_name"

#saving json file with new R .net file and simulation label
sed -e "s/filename.net/$file_name/g" -e "s/simulationname/$prefix/g" "$json_file" > "$new_json_file_name"
echo "$new_json_file_name was created"
echo "Compute the simulation"
$dir_home$exec_code $dir_home$new_json_file_name

#Moving csv files to directory so that we can check
mv "$dir_home/statistic.$prefix.csv"  $dir_out
if [ -f "$dir_home/agent.$prefix.ppm" ]; then
    mv "$dir_home/agent.$prefix.ppm"  $dir_out
fi
#mv "$dir_home*.ppm"  $dir_out
cp -f "$file_name"  $dir_out

echo "<END-SIMULATION>" >> result.txt
