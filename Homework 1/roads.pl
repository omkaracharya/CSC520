%==============================================================================
%  
%   Weighted graph of Romanian road system.
%   
%==============================================================================
road(oradea,    zerind,     71).  road(zerind,         arad,           75).
road(arad,      timisoara, 118).  road(timisoara,      lugoj,         111).
road(lugoj,     mehadia,    70).  road(dobreta,        mehadia,        75).
road(oradea,    sibiu,     151).  road(arad,           sibiu,         140).
road(dobreta,   craiova,   120).  road(sibiu,          rimnicu_vilcea, 80).
road(sibiu,     fagaras,    99).  road(rimnicu_vilcea, craiova,       146).
road(pitesti,   craiova,   138).  road(rimnicu_vilcea, pitesti,        97).
road(bucharest, pitesti,   101).  road(bucharest,      fagaras,       211).
road(bucharest, giurgiu,    90).  road(bucharest,      urziceni,       85).
road(vaslui,    urziceni,  142).  road(hirsova,        urziceni,       98).
road(hirsova,   eforie,     86).  road(vaslui,         iasi,           92).
road(neamt,     iasi,       87).

twoWayRoad(City1, City2, Distance):-
      road(City1, City2, Distance).
twoWayRoad(City1, City2, Distance):-
      road(City2, City1, Distance).

move(City1, City2):- twoWayRoad(City1, City2, _).

move(City1, City2, Distance):-
      twoWayRoad(City1, City2, Distance).

%==============================================================================
%  
%   Decimal latitude and longitude of Romanian cities.
%   Useful in computing D-2 (Pythagorian) distance in the heuristic.
%
%   0.00001 degrees is about 1 m precision, ridiculous overkill for a city-to-city
%   heuristic.
%
%   city(name, latitude north, longitude east)
%   
%==============================================================================
city(arad,           46.16667, 21.3).
city(bucharest,      44.41667, 26.1).
city(craiova,        44.33333, 23.81667).
city(dobreta,        44.5,     23.95).
city(eforie,         44.06667, 28.63333).
city(fagaras,        45.84472, 24.97417).
city(giurgiu,        43.90083, 25.97389).
city(hirsova,        46.68333, 27.53333).
city(iasi,           47.16222, 27.58889).
city(lugoj,          45.68611, 21.90056).
city(mehadia,        44.90083, 22.35028).
city(neamt,          46.9275,  26.37083).
city(oradea,         47.07222, 21.92111).
city(pitesti,        44.86056, 24.86778).
city(rimnicu_vilcea, 45.10472, 24.37556).
city(sibiu,          45.79583, 24.15222).
city(timisoara,      45.75972, 21.23).
city(urziceni,       44.71806, 26.64528).
city(vaslui,         46.63833, 27.72917).
city(zerind,         46.63333, 21.66667).

