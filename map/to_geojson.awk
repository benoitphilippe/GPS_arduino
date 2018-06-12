#!/usr/bin/awk -f

# Take a simple acquisition file with format :
#
# LAT LON date time
#
# this program simply write it to geoJSON
#
# how tu use it
# ------------------------------------
# ./to_geoJSON in_file > out_file
#

BEGIN {
    print "{ \"type\": \"Feature\",\n\"geometry\": {\n\"type\": \"LineString\",\n\"coordinates\": [";
    watcher=0
}
/[0-9]+/ {
    if (watcher == 0){
        print "[", $2, ",", $1, "]";
        watcher++
    }
    else {
        print ",", "[", $2, ",", $1, "]"
    }
}
END {
    print "]\n}\n}\n"
}
