#include <cmath> 
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

#define pi 3.14159265358979323846
#define earthRadiusKm 6371.0

// This function converts decimal degrees to radians
double deg2rad(double deg) {
  return (deg * pi / 180);
}

//  This function converts radians to decimal degrees
double rad2deg(double rad) {
  return (rad * 180 / pi);
}

/**
 * Returns the distance between two points on the Earth.
 * Direct translation from http://en.wikipedia.org/wiki/Haversine_formula
 * @param lat1d Latitude of the first point in degrees
 * @param lon1d Longitude of the first point in degrees
 * @param lat2d Latitude of the second point in degrees
 * @param lon2d Longitude of the second point in degrees
 * @return The distance between the two points in kilometers
 */
double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d) {
  double lat1r, lon1r, lat2r, lon2r, u, v;
  lat1r = deg2rad(lat1d);
  lon1r = deg2rad(lon1d);
  lat2r = deg2rad(lat2d);
  lon2r = deg2rad(lon2d);
  u = sin((lat2r - lat1r)/2);
  v = sin((lon2r - lon1r)/2);
  return 2.0 * earthRadiusKm * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
}



map<string, pair<double,double> > readGeoData(char* fileName){
	
	std::ifstream f(fileName);
	std::string line = "";

	map<string, pair<double,double> > sites_lat_long;
	
	
	if (!f){
		cout<<"problem in opening the file: "<<fileName<<endl;
		
	}else 
	{
		getline(f, line);
		
		while (getline(f, line)) 
		{
			string word = "";
			string siteName = "";
			string latitude = "";
			string longitude = "";
			int wordCount=0;
			stringstream strstr(line);		
				
			while (getline(strstr,word, ',')){
				if (wordCount == 0){   //site code
				 siteName = word;
				}
				if (wordCount == 1){  // latitude
				 latitude = word;
				}
				if (wordCount == 2){ // longitude
					longitude = word;
					break;
				}		

				++wordCount;
			}
			cout<<siteName<<"\t"<<latitude<<"\t"<<longitude<<endl;
			sites_lat_long[siteName] = make_pair(stod(latitude),stod(longitude));
		}
	}
	
	return sites_lat_long;
}


void calcDist(map<string, pair<double,double> >  data, char* fileName){
	
	string opFile(fileName);
	opFile = opFile.substr(0,opFile.find("."));
	ofstream geoF(opFile+"_geoDist.csv");
	
	map<string, pair<double,double> >::iterator itr1;
	map<string, pair<double,double> >::iterator itr2;
	double dist = 0;

	geoF<<"SiteID,SiteID,distance"<<endl;		

	for (itr1 = data.begin(); itr1 != --data.end(); ++itr1){
		for (itr2 = next(itr1,1); itr2 != data.end(); ++itr2){
			dist = distanceEarth(itr1->second.first,itr1->second.second,itr2->second.first,itr2->second.second);
			cout<<itr1->first<<","<<itr2->first<<","<<dist<<endl;		
			geoF<<itr1->first<<","<<itr2->first<<","<<dist<<endl;		
		}
	}
	
	geoF.close();
}

int main(int argc, char **argv){
	
	char* fileName = argv[1];
	
	map<string, pair<double,double> > site_lat_long = readGeoData(fileName);
	calcDist(site_lat_long, fileName);
		

	return 0;
	
}
