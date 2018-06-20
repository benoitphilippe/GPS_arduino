#!/usr/bin/env python3
#encoding=utf-8

import json
import sys
import requests

def follow_road(file_name, out_file_name, step=50):
    """
        take a geoJSON file, from all coordinates, get the road that could be followed
        file_name : string for a geoJSON file 
        step : integer, number of point to use for algorithme
        """
    
    with open(file_name, 'r') as f:
        journey = json.load(f)
    if "geometry" in journey:
        # create the final coordinates
        road = json.loads('{ "type": "Feature","geometry": {"type": "LineString","coordinates": []}}')
        last_coords = journey["geometry"]["coordinates"][0]
        for i in range(step,len(journey["geometry"]["coordinates"]), step):
            coords = journey["geometry"]["coordinates"][i]
            request_string = "http://router.project-osrm.org/route/v1/driving/{0:.6f},{1:.6f};{2:.6f},{3:.6f}?geometries=geojson&overview=full".format(
                last_coords[0], last_coords[1], coords[0], coords[1]
            )
            last_coords = coords
            r = requests.get(request_string)
            if r.status_code == 200:
                datas = r.json()
            else:
                r.raise_for_status()
            for road_coords in datas["routes"][0]["geometry"]["coordinates"]:
                road["geometry"]["coordinates"].append(road_coords)
        # last request for remaining coords
        coords = journey["geometry"]["coordinates"][len(journey["geometry"]["coordinates"]) - 1] # last coords
        request_string = "http://router.project-osrm.org/route/v1/driving/{0:.6f},{1:.6f};{2:.6f},{3:.6f}?geometries=geojson&overview=full".format(
            last_coords[0], last_coords[1], coords[0], coords[1]
        ) 
        r = requests.get(request_string)
        if r.status_code == 200:
            datas = r.json()
        else:
            r.raise_for_status()
        for road_coords in datas["routes"][0]["geometry"]["coordinates"]:
            road["geometry"]["coordinates"].append(road_coords)
        with open(out_file_name, 'w') as fout:
            json.dump(road, fout)
            print("done :)")
    else:
        print("wrong file format")    
            
            


if __name__ == "__main__":
    if len(sys.argv) == 3:
        follow_road(sys.argv[1], sys.argv[2])
    elif len(sys.argv) == 4:
        follow_road(sys.argv[1], sys.argv[2],step=int(sys.argv[3]))
    else:
        print("wrong number of argument !")