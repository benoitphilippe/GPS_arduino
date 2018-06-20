#!/usr/bin/env python3
#encoding=utf-8

from math import radians, cos, sin, asin, sqrt
import sys
import numpy as np
import json
import matplotlib.pyplot as plt
from math import sqrt, floor

def haversine(lon1, lat1, lon2, lat2):
    """
    Calculate the great circle distance between two points 
    on the earth (specified in decimal degrees)
    """
    # convert decimal degrees to radians 
    lon1, lat1, lon2, lat2 = map(radians, [lon1, lat1, lon2, lat2])
    # haversine formula 
    dlon = lon2 - lon1 
    dlat = lat2 - lat1 
    a = sin(dlat/2)**2 + cos(lat1) * cos(lat2) * sin(dlon/2)**2
    c = 2 * asin(sqrt(a)) 
    # Radius of earth in kilometers is 6371
    km = 6371* c
    meters = km * 1000
    return meters

def get_trajectory_distance(lon, lat, feature):
    """ Calcul la distance en metre entre un point et un trajectoire
        lon : float la longitude du point
        lat : float la latitude du point
        feature : un objet geojson possédant une geometry linstring avec des coordinates
    """
    coordinates = feature["geometry"]["coordinates"]
    nearest_point_id = 0
    distance_nearest_point = 10000
    for i in range(len(coordinates)):
        if haversine(lon, lat, coordinates[i][0], coordinates[i][1]) < distance_nearest_point:
            distance_nearest_point = haversine(lon, lat, coordinates[i][0], coordinates[i][1])
            nearest_point_id = i
    # get our nearest point id
    # estimate either previous or next point is nearest
    if nearest_point_id >= len(coordinates)-1:
        id_next_nearest = nearest_point_id - 1
    elif nearest_point_id == 0:
        id_next_nearest = nearest_point_id + 1
    else :    
        distance_next_point = haversine(lon, lat, coordinates[nearest_point_id + 1][0], coordinates[nearest_point_id +1][1])   
        distance_previous_point = haversine(lon, lat, coordinates[nearest_point_id -1][0], coordinates[nearest_point_id-1][1])   
        if distance_next_point > distance_previous_point:
            id_next_nearest = nearest_point_id - 1
        else:
            id_next_nearest = nearest_point_id + 1
    # on a les deux points les plus proches
    # on a un triangle ABC dont A est le point dont il faut estimer la distance
    # on retrouve les longueurs AB, BC, CA
    # on a le point H dans la longueur BC
    # on veut estimer la longueur AH pour notre erreur
    # Pour ça, on a AH² = AB² - ((BC² - AC² + AB²)/(2 * BC))²

    AB = haversine(lon, lat, coordinates[nearest_point_id][0], coordinates[nearest_point_id][1])
    BC = haversine(coordinates[id_next_nearest][0], coordinates[id_next_nearest][1], coordinates[nearest_point_id][0], coordinates[nearest_point_id][1])
    CA = haversine(lon, lat, coordinates[id_next_nearest][0], coordinates[id_next_nearest][1])
    # il faut gérer le cas où BC = 0
    if BC <= 0.05:
        AH = AB
    else:
        AH = sqrt(AB**2 - ((BC**2 - CA**2 + AB**2)/(2 * BC))**2)
    return AH

def extend_size(tab, size):
    """ extend a tab to size by duplicating some datas"""
    # premièrement il faut obtenir le ratio
    if len(tab) == size:
        return tab
    ratio = size / len(tab)
    pointer_tab = 0
    ratio_state = 0
    # avec ce ration on créer un nouvelle tab
    tab_resized = []
    for i in range(size):
        tab_resized.append(tab[pointer_tab])
        if floor(ratio_state) < i:
            ratio_state += ratio
            pointer_tab += 1
            if pointer_tab >= len(tab):
                pointer_tab -= 1
    return tab_resized


if __name__ == "__main__":
    if len(sys.argv) > 2:
        # le premier fichier est le fichier geoJSON de reference
        journeys = []
        with open(sys.argv[1]) as f:
            reference = json.load(f)
        for i in range(2, len(sys.argv)):
            with open(sys.argv[i]) as f:
                journeys.append(json.load(f))
        # get references and journey

        # get max size taget
        ID_longest_traget = 0
        len_coordinates_max = 0
        for i in range(len(journeys)):
            if len(journeys[i]["geometry"]["coordinates"]) > len_coordinates_max:
                ID_longest_traget = i
                len_coordinates_max = len(journeys[i]["geometry"]["coordinates"]) 
        for i in range(len(journeys)):
            resized_tab_lon = []
            resized_tab_lat = []
            tab_lon = []
            tab_lat = []
            for coordinate in journeys[i]["geometry"]["coordinates"]:
                tab_lon.append(coordinate[0])
                tab_lat.append(coordinate[1])
            resized_tab_lon = extend_size(tab_lon, len_coordinates_max)
            resized_tab_lat = extend_size(tab_lat, len_coordinates_max)
            journeys[i]["geometry"]["coordinates"]  = []
            for j in range(len_coordinates_max):
                journeys[i]["geometry"]["coordinates"].append([resized_tab_lon[j], resized_tab_lat[j]])

        plt.suptitle("Erreur de positionnement sur le trajet \"Autour de la rocade\"")
        plt.title("comparé aux coordonnées de Open Street Map")
        plt.ylabel("erreur de positionnement en mettres")
        plt.xlabel("points")
        legend_label = ['200 ms rocade sous la pluie', '2s rocade sous la pluie', '2s rocade beau temps']

        for k in range(len(journeys)):
            errors = []
            distance = []
            total_distance = 0
            # calcul de la distance
            j = 0
            last_coordinate = journeys[k]["geometry"]["coordinates"][0]
            for coordinate in journeys[k]["geometry"]["coordinates"]:
                j += 1
                error = get_trajectory_distance(coordinate[0], coordinate[1], reference)
                if error > 50:
                    errors.append(50)
                else :
                    errors.append(error)
                total_distance += haversine(last_coordinate[0], last_coordinate[1], coordinate[0], coordinate[1])
                distance.append(total_distance)
            
            print(sum(errors) / len(errors))
            plt.plot(errors, label=legend_label[k])
        plt.legend()
        plt.show()
    else :
        print("wrong arguments:")
        print("error.py Reference.geojson journey.geojson [journey.geojson]")
