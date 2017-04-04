#include <stdio.h>
#include <fstream>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <string>
#include <iostream>
#include <sstream>
#include <istream>
#include <algorithm>
#include <functional>
#include <math.h>
#include <iomanip>
using namespace std;



map<pair<string,string>,double> readGeoDist(char* fileName){
	
	
	ifstream geoFile(fileName);
	map<pair<string,string>,double> geoData;
	
	if (!geoFile) cout<<"File error!!\t"<<fileName<<endl;
	else{
			string line;
			string siteX,siteY;
			double distance;
			int wordCount = 0;
			string word;
			getline(geoFile,line);
			while (getline(geoFile,line)){
				word = "";
				wordCount = 0;
				stringstream str(line);
				while (getline(str,word,',')){
					if (wordCount ==0){
						siteX = word;
						++wordCount;
					}
					else if (wordCount ==1){
						siteY = word;
						++wordCount;
					}
					else if (wordCount == 2){
						distance = stod(word);
						wordCount++;
					}
					else break;
				}
				geoData.insert(make_pair(make_pair(siteX,siteY),distance));
			}
		
	}
	
	return geoData;
}


map<string,map<int, map<string,int> > > cascadesAllDatesFreq(char* fileName, char* g){
	
	std::ifstream f(fileName);
	map<string, map<int,map<string, int> > > grapheme_date_site_freq;  // grapheme, date of inscriptions, list of distinct sites and frequency of insciption that inscribed.


	
	if (!f){
		cout<<"problem in opening the file: "<<fileName<<endl;
		
	}else 
	{
		std::string line = "";
		getline(f, line); //ignoring headers
		int rowid = 0;
		
		while (getline(f, line)) 
		{
			string word = "";
			string graphemeCode = "";
			string siteCode = "";
			string timestamp = "";
			int wordCount=0;
			
			int year = 0;
			int month = 0; 
			int day = 0;
			
			string formattedDate = "";
			
			stringstream strstr(line);		
				
			while (getline(strstr,word, ',')){
				if (wordCount == 0){   //grapheme code
					graphemeCode = word;
				}
				if (wordCount == 1){  // glyph block site
					siteCode = word;
				}
				if (wordCount == 2){ // timestamp
					timestamp = word;
				}
				++wordCount;
			}
			++rowid;
	
			//grapheme date to site with the frequency of inscriptions data 
			if (grapheme_date_site_freq.find(graphemeCode) == grapheme_date_site_freq.end()){
				map<string, int> site_freq;
				site_freq.insert(make_pair(siteCode,1));
				map<int,map<string, int> > time_sites_freq;
				time_sites_freq.insert(make_pair(stoi(timestamp),site_freq));
				grapheme_date_site_freq.insert(make_pair(graphemeCode,time_sites_freq));
			}
			else if (grapheme_date_site_freq[graphemeCode].find(stoi(timestamp)) == grapheme_date_site_freq[graphemeCode].end()){
				map<string, int> site_freq;
				site_freq.insert(make_pair(siteCode,1));
				grapheme_date_site_freq[graphemeCode].insert(make_pair(stoi(timestamp),site_freq));
			}
			else if (grapheme_date_site_freq[graphemeCode][stoi(timestamp)].find(siteCode) == grapheme_date_site_freq[graphemeCode][stoi(timestamp)].end()){
				grapheme_date_site_freq[graphemeCode][stoi(timestamp)].insert(make_pair(siteCode,1));
			}
			else{
				 ++grapheme_date_site_freq[graphemeCode][stoi(timestamp)][siteCode];
			}
		}
		/* ***************** testing all frequencies site inscription records for each grapheme ************/
		
		map<string,map<int, map<string,int> > >::iterator itr6;
		map<int, map<string,int> > ::iterator itr7;
		map<string,int>::iterator itr8;
		
		ofstream gInscSitesF(string(g)+"_InscT_sitesFreq.csv");
		string sites_freqs = "";

		int totalInsc = 0;
		itr6 = grapheme_date_site_freq.find(string(g));
	//	for (itr3 = grapheme_date_site_freq.begin(); itr3 != grapheme_date_site_freq.end(); ++itr3){
			for (itr7 = itr6->second.begin(); itr7 != itr6->second.end(); ++itr7){
				cout<<itr6->first<<"\t"<<itr7->first<<"\t";
				gInscSitesF<<itr6->first<<","<<itr7->first<<",";
				totalInsc = 0;
				sites_freqs = "";
				for (itr8 = itr7->second.begin(); itr8 != --itr7->second.end(); ++itr8){
					cout<<itr8->first<<","<<itr8->second<<"\t";
					sites_freqs.append(itr8->first+","+to_string(itr8->second)+",");
					totalInsc += itr8->second;

				}
				itr8 = --itr7->second.end();
				totalInsc += itr8->second;
				cout<<itr8->first<<","<<itr8->second<<"\t"<<totalInsc<<endl; 
				sites_freqs.append(itr8->first+","+to_string(itr8->second)+"\n");
				gInscSitesF<<totalInsc<<","<<sites_freqs;
			}
		//}
		
		gInscSitesF.close();
	}
		
	return grapheme_date_site_freq;
}




map<string,vector<string> > buildCascade_metaData (map<string,map<int, map<string,int> > > graphemeInscriptions, map<pair<string,string>,double> geoData, char* g){
	
	
	map<string,vector<string> > cascade;
	map<string,map<int, map<string,int> > >::iterator itr1;
	map<int, map<string,int> >::iterator itr2;
	map<string,int>::iterator itr3;
	map<string,vector<string> >::iterator itr4;
	map<string,pair<int,int> > influenced;
	map<string,pair<int, int> > recentlyInfluenced;  // the site and number of times they inscribed in the previous timestep 
	map<string,pair<int, int> >::iterator itr5;
	queue<string> influenceOrder; 
	
	/*  all times of inscription */
	
	itr1 = graphemeInscriptions.find(string(g)); // grapheme 
	//cout<<itr1->first<<":\n";
	for (itr2 = itr1->second.begin(); itr2 != itr1->second.end(); ++itr2){ //time
	//	cout<<itr2->first<<endl;
	}
	
	/* ***********  mean and standard deviation of distance *********** */
	
	itr1 = graphemeInscriptions.find(string(g)); // grapheme 
	
	/* first get the list of sites inscribing the grapheme */
	map<string,int> sites; //easier to use map to avoid duplicity incase of sites that have inscribed the grapheme more than once
	for (itr2 = itr1->second.begin(); itr2 != itr1->second.end(); ++itr2){ //time (need to get through the entire timeline to pick up the list)
		for(itr3 = itr2->second.begin(); itr3 != itr2->second.end(); ++itr3){ // all the sites inscribing in the timestamp
			sites[itr3->first] = 1;
		}
	} 
	
	/* ***** Check for exclusion of sites with no geo information ****** */
/*	
	map<string, int> excludedSites;
	
	for (itrSiteX = sites.begin(); itrSiteX != sites.end(); ++itrSiteX){
		for (itrSiteY = sites.begin(); itrSiteY != sites.end(); ++itrSiteY){
			if ((geoData[make_pair(itrSiteX->first,itrSiteY->first)] ==0) && (geoData[make_pair(itrSiteY->first,itrSiteX->first)] ==0)) {
				excludedSites
			}
		}
	}
	
	*/	
	double mean_dist = 0;
	double sd_dist = 0;	
	double min_dist = numeric_limits<double>::max();
	double max_dist = 0.0;
		
	map<string,int>::iterator itrSiteX;
	map<string,int>::iterator itrSiteY;
	
	for (itrSiteX = sites.begin(); itrSiteX != sites.end(); ++itrSiteX){
		for (itrSiteY = sites.begin(); itrSiteY != sites.end(); ++itrSiteY){
			if ((geoData[make_pair(itrSiteX->first,itrSiteY->first)]!=0)) {
				mean_dist += geoData[make_pair(itrSiteX->first,itrSiteY->first)];
				if (geoData[make_pair(itrSiteX->first,itrSiteY->first)] <min_dist) min_dist = geoData[make_pair(itrSiteX->first,itrSiteY->first)];
				if (geoData[make_pair(itrSiteX->first,itrSiteY->first)] > max_dist) max_dist = geoData[make_pair(itrSiteX->first,itrSiteY->first)];	
			//	cout<<itrSiteX->first<<"\t"<<itrSiteY->first<<":\t"<<geoData[make_pair(itrSiteX->first,itrSiteY->first)]<<endl;
			}
		}
	}
	
	mean_dist = 2*mean_dist/(sites.size()*(sites.size()-1));
	
	cout<<"# of sites:"<<sites.size()<<"\t"<<"Mean distance: "<<mean_dist<<endl;
	cout<<"Min distance:"<<min_dist<<"\t"<<"Max distance: "<<max_dist<<endl;
	
	/* ***Now the standard deviation! *** */
	
	for (itrSiteX = sites.begin(); itrSiteX != sites.end(); ++itrSiteX){
		for (itrSiteY = sites.begin(); itrSiteY != sites.end(); ++itrSiteY){
			sd_dist += pow((geoData[make_pair(itrSiteX->first,itrSiteY->first)]-mean_dist),2);
			
		}
	}
	
	sd_dist = sqrt(2*sd_dist/(sites.size()*(sites.size()-1)));
	
	cout<<"SD distance: "<<sd_dist<<endl;
	
	/* ***********  mean and standard deviation of distance end! *********** */
	


/* **************** calculate minimum time difference between sites ***************************** */

	
	map<pair<string,string>,vector<double> > minTimeDiff;
	
	itr1 = graphemeInscriptions.find(string(g)); // grapheme 
	itr2 = itr1->second.begin();  //  time of inscription
	
	int sourceTime = 0;
	int destTime = 0;
	
	for(itr3 = itr2->second.begin(); itr3 != itr2->second.end(); ++itr3){
		cascade[itr3->first].push_back(to_string(itr2->first));  // first adopter: site code + time 
		cascade[itr3->first].push_back("NULL");                      //parent 
		cascade[itr3->first].push_back("NULL");							// parent time of adoption
		cascade[itr3->first].push_back("NULL");                      //time difference between parent and influenced site
		cascade[itr3->first].push_back("NULL");                      //probability of influence based on time only
	//	cascade[itr3->first].push_back("NULL");                      //distance from parent
		//cascade[itr3->first].push_back("NULL");                      //probability of influence based on time and distance
		influenced.insert(make_pair(itr3->first, make_pair(itr2->first, itr3->second)));    // add to the list of latest potential source of influence. The 2nd parameter (pair) is the time + the number of times the grapheme is inscribed by this source in this time. 
		influenceOrder.push(itr3->first);
	} 
	
	
	for (itr2 = ++itr1->second.begin(); itr2 != itr1->second.end(); ++itr2){ //time
		destTime = itr2->first;
		for(itr3 = itr2->second.begin(); itr3 != itr2->second.end(); ++itr3){ // all the sites inscribing at the time
			if (cascade.find(itr3->first) == cascade.end()){
				cascade[itr3->first].push_back(to_string(destTime));    //new infected site & the time it was infected
				influenceOrder.push(itr3->first);						// the order of influence
				recentlyInfluenced.insert(make_pair(itr3->first,make_pair(itr2->first,itr3->second))); //pool of new sites influenced in this time
				for (itr5 = influenced.begin(); itr5 != influenced.end(); ++itr5){
					cascade[itr3->first].push_back(itr5->first); 						 //parents of the infected site (all sites active in the previous times)	
					cascade[itr3->first].push_back(to_string(itr5->second.first)); 						 //time of parent site when it infected the newer site	
					cascade[itr3->first].push_back(to_string(destTime - itr5->second.first));    // time difference between the parent and infected site
					cascade[itr3->first].push_back(to_string(0));    // probability of influence based on time only
					cascade[itr3->first].push_back(to_string(0));    // probability of influence based on time normalized by mean only
					cascade[itr3->first].push_back(to_string(0));    // probability of influence based on time normalized by min max only
					cascade[itr3->first].push_back(to_string(0));    // probability of influence based on time only
					cascade[itr3->first].push_back(to_string(0));    // probability of influence based on time normalized by mean only
					cascade[itr3->first].push_back(to_string(0));    // probability of influence based on time normalized by min max only
					cascade[itr3->first].push_back(to_string(0));    // probability of influence based on time only
					cascade[itr3->first].push_back(to_string(0));    // probability of influence based on time normalized by mean only
					cascade[itr3->first].push_back(to_string(0));    // probability of influence based on time normalized by min max only
					
					
					pair<string, string> p = make_pair(itr5->first, itr3->first);				// to save the minimum time between parent and infected
					minTimeDiff[p].push_back(destTime - itr5->second.first);								// this map saves pairwise shortest time to infect
	/*				if (geoData.find(make_pair(itr3->first,itr5->first)) != geoData.end()){
						cascade[itr3->first].push_back(to_string(geoData[make_pair(itr3->first,itr5->first)])); //distance of parent and influenced site
					
					}
					else {
						cascade[itr3->first].push_back(to_string(geoData[make_pair(itr5->first,itr3->first)])); //distance of parent and influenced site
					
					}
	*/
				}
			}
			else {
				recentlyInfluenced.insert(make_pair(itr3->first,make_pair(itr2->first,itr3->second)));  // the current site has already adopted. So its not added to the cascade list. However, its most recent inscription can potentially influence sites in the next timestep.
			}
		}
		
		for (itr5 = recentlyInfluenced.begin(); itr5 != recentlyInfluenced.end(); ++itr5){
			if (influenced.find(itr5->first) == influenced.end()){
				influenced.insert(*itr5);
				
			}
			else{
				influenced[itr5->first].first = itr5->second.first;
				influenced[itr5->first].second += itr5->second.second;

			}
		}
		recentlyInfluenced.clear();
	}
	
	
	
/* **************** calculate minimum time difference between sites ***************************** */	


	
/* *****  mean and sd of time calculation! *************** */


	double mean_time;
	double sd_time = 0.0;
	double min_time = numeric_limits<double>::max();
	double max_time = 0.0;
	
	cout << "minimum time differences\n";
	
	map<pair<string,string>,vector<double> >::iterator itrMinT;
	
	for (itrMinT = minTimeDiff.begin(); itrMinT != minTimeDiff.end(); ++itrMinT){
	//cout<<itrMinT->first.first<<"->"<<itrMinT->first.second<<":"<<itrMinT->second<<endl;
		mean_time += itrMinT->second[0];
		if (itrMinT->second[0] <min_time) min_time = itrMinT->second[0];
		if (itrMinT->second[0] > max_time) max_time = itrMinT->second[0];
	}
	
	mean_time = mean_time/minTimeDiff.size();

	cout<<"Mean time:"<<mean_time<<endl;
	
	cout<<"max time: "<<max_time<<endl;
	cout<<"min time: "<<min_time<<endl;

/* ***Now the standard deviation for minimum time! *** */
	
		for (itrMinT = minTimeDiff.begin(); itrMinT != minTimeDiff.end(); ++itrMinT){
			sd_time += pow((itrMinT->second[0]-mean_time),2);
		}
	
	sd_time = sqrt(sd_time/minTimeDiff.size());
	
	cout<<"SD time: "<<sd_time<<endl;

	
	
/* *****  END: mean and sd of time calculation end! *************** */



/* *****  probabilities of influence based on exponential decay. time, distance,  and time + distance model (assuming time and distance are independent distributions *********** */

	float lambdaT = 0; //rT;
	float lambdaD = 0;  //rD;
	
		for (itrMinT = minTimeDiff.begin(); itrMinT != minTimeDiff.end(); ++itrMinT){
			lambdaT += (itrMinT->second[0]-min_time)/(max_time-min_time);
		}
		lambdaT = lambdaT/minTimeDiff.size();
		cout<<lambdaT<<endl;
		cout<<1/lambdaT<<endl;

		cout<<min_dist<<endl;
		cout<<max_dist<<endl;
	

	for (itrSiteX = sites.begin(); itrSiteX != sites.end(); ++itrSiteX){
		for (itrSiteY = sites.begin(); itrSiteY != sites.end(); ++itrSiteY){
			if ((geoData[make_pair(itrSiteX->first,itrSiteY->first)]!=0)) {
			lambdaD += (geoData[make_pair(itrSiteX->first,itrSiteY->first)]-min_dist)/(max_dist-min_dist);
		
			}
		}
	}
	
	lambdaD = 2*lambdaD/(sites.size()*(sites.size()-1));

		cout<<lambdaD<<endl;
		cout<<1/lambdaD<<endl;


	lambdaT = 1/lambdaT; //rT;
	lambdaD = 1/lambdaD;  //rD;
		
	double probInfT0 = 0.0;
	double probInfD0 = 0.0;
	double probInfTD0 = 0.0;
	
	
	double probInfT = 0.0;
	double probInfD = 0.0;
	double probInfTD = 0.0;
	
	double probInfT1 = 0.0;
	double probInfD1 = 0.0;
	double probInfTD1 = 0.0;
	
	string opFileName = string(g)+"_"+to_string(influenceOrder.size())+"_"+to_string(lambdaT)+"_"+to_string(lambdaD)+"_AllEdgesCascade.csv";
	ofstream opFile(opFileName);	
	
	opFile<<"From,To,TDiff,inf,meanT,sdT,InfMean,minT,maxT,InfMinMax,dist,InfD,meanD,sdD,InfMeanD,minD,maxD,InfMinMaxD,InfTD,InfMeanTD,InfMinMaxTD\n"; 
	
	
	for (itrMinT = minTimeDiff.begin(); itrMinT != minTimeDiff.end(); ++itrMinT){
				
		if (geoData[make_pair(itrMinT->first.first,itrMinT->first.second)]  != 0){
		
			probInfT0 = lambdaT * exp(-lambdaT*(itrMinT->second[0]));
			probInfT = lambdaT * exp(-lambdaT*((itrMinT->second[0]-mean_time)/sd_time));
			probInfT1 = lambdaT * exp(-lambdaT*((itrMinT->second[0]-min_time)/(max_time-min_time)));
		
			itrMinT->second.push_back(probInfT0);
			itrMinT->second.push_back(probInfT);
			itrMinT->second.push_back(probInfT1);
			
			probInfD0 = lambdaD * exp(-lambdaD*(geoData[make_pair(itrMinT->first.first,itrMinT->first.second)]));
			probInfD = lambdaD * exp(-lambdaD*((geoData[make_pair(itrMinT->first.first,itrMinT->first.second)]-mean_dist)/sd_dist));
			probInfD1 = lambdaD * exp(-lambdaD*((geoData[make_pair(itrMinT->first.first,itrMinT->first.second)]-min_dist)/(max_dist-min_dist)));
			
			itrMinT->second.push_back(probInfD0);
			itrMinT->second.push_back(probInfD);
			itrMinT->second.push_back(probInfD1);
			
			probInfTD0 = probInfT0*probInfD0;
			probInfTD = probInfT*probInfD;
			probInfTD1 = probInfT1*probInfD1;
			
			itrMinT->second.push_back(probInfTD0);
			itrMinT->second.push_back(probInfTD);
			itrMinT->second.push_back(probInfTD1);
			
			//	cout<<itrMinT->first.first<<","<<itrMinT->first.second<<","<<itrMinT->second<<","<<probInfT<<","<<mean_time<<","<<sd_time<<","<<geoData[make_pair(itrMinT->first.first,itrMinT->first.second)]<<","<<probInfD<<","<<mean_dist<<","<<sd_dist<<","<<probInfTD<<","<<max_time<<","<<min_time<<","<<(itrMinT->second-min_time)/(max_time-min_time)<<","<<(-lambdaT*((itrMinT->second-min_time)/(max_time-min_time)))<<","<<exp(-lambdaT*((itrMinT->second-min_time)/(max_time-min_time)))<<","<<(lambdaT*exp(-lambdaT*((itrMinT->second-min_time)/(max_time-min_time))))<<endl;
			opFile<<itrMinT->first.first<<","<<itrMinT->first.second<<","<<itrMinT->second[0]<<","<<probInfT0<<","<<mean_time<<","<<sd_time<<","<<probInfT<<","<<min_time<<","<<max_time<<","<<probInfT1<<","<<geoData[make_pair(itrMinT->first.first,itrMinT->first.second)]<<","<<probInfD0<<","<<mean_dist<<","<<sd_dist<<","<<probInfD<<","<<min_dist<<","<<max_dist<<","<<probInfD1<<","<<probInfTD0<<","<<probInfTD<<","<<probInfTD1<<endl;
		}
		else if (geoData[make_pair(itrMinT->first.second,itrMinT->first.first)]  != 0){ 
			probInfD0 = lambdaD * exp(-lambdaD*(geoData[make_pair(itrMinT->first.second,itrMinT->first.first)]));
			probInfD = lambdaD * exp(-lambdaD*((geoData[make_pair(itrMinT->first.second,itrMinT->first.first)]-mean_dist)/sd_dist));	
			probInfD1 = lambdaD * exp(-lambdaD*((geoData[make_pair(itrMinT->first.second,itrMinT->first.first)]-min_dist)/(max_dist-min_dist)));
			
			itrMinT->second.push_back(probInfD0);
			itrMinT->second.push_back(probInfD);
			itrMinT->second.push_back(probInfD1);
			
			probInfTD0 = probInfT0*probInfD0;
			probInfTD = probInfT*probInfD;
			probInfTD1 = probInfT1*probInfD1;
			
			itrMinT->second.push_back(probInfTD0);
			itrMinT->second.push_back(probInfTD);
			itrMinT->second.push_back(probInfTD1);
			
			//	cout<<itrMinT->first.first<<","<<itrMinT->first.second<<","<<itrMinT->second<<","<<probInfT<<","<<mean_time<<","<<sd_time<<","<<geoData[make_pair(itrMinT->first.second,itrMinT->first.first)]<<","<<probInfD<<","<<mean_dist<<","<<sd_dist<<","<<probInfTD<<","<<max_time<<","<<min_time<<","<<(itrMinT->second-min_time)/(max_time-min_time)<<","<<(-lambdaT*((itrMinT->second-min_time)/(max_time-min_time)))<<","<<exp(-lambdaT*((itrMinT->second-min_time)/(max_time-min_time)))<<","<<(lambdaT*exp(-lambdaT*((itrMinT->second-min_time)/(max_time-min_time))))<<endl;
			opFile<<itrMinT->first.first<<","<<itrMinT->first.second<<","<<itrMinT->second[0]<<","<<probInfT0<<","<<mean_time<<","<<sd_time<<","<<probInfT<<","<<min_time<<","<<max_time<<","<<probInfT1<<","<<geoData[make_pair(itrMinT->first.second,itrMinT->first.first)]<<","<<probInfD0<<","<<mean_dist<<","<<sd_dist<<","<<probInfD<<","<<min_dist<<","<<max_dist<<","<<probInfD1<<","<<probInfTD0<<","<<probInfTD<<","<<probInfTD1<<endl;
		
		}
		
		else{
			cout<<"Missing distance: "<<itrMinT->first.second<<","<<itrMinT->first.first<<endl;
			
		}
		
		
	}

	opFile.close();
/* *****  END: probabilities of influence based on exponential decay. time, distance,  and time + distance model (assuming time and distance are independent distributions *********** */



	
//	cout<<"************************display cascade order*************************************"<<endl;

//	cout<<influenceOrder.size()<<endl; 
//	string opFileName = string(g)+"_"+to_string(influenceOrder.size())+"_"+to_string(r)+"_cascade.csv";
	//ofstream opFile(opFileName);

	//cout<<"Parent\t Parent_Time\t InfluencedSite\t InfSite_time\t diff_days\t mean_time \t sd_time \t prob\t prob_inf_t\t dist\t mean_dist \t sd_dist \t prob_inf_t_d\n";

	string influencedSite = "";

	while (!influenceOrder.empty()){
		influencedSite = influenceOrder.front();
		itr4 = cascade.find(influencedSite);
		if (itr4 != cascade.end()){
			//cout<<itr4->first<<"\t"<<itr4->second[0]<<":\t"<<itr4->second.size()<<endl;
			for (int i = 1; i < itr4->second.size(); i+=12){
				if (minTimeDiff.find(make_pair(itr4->second[i],itr4->first)) != minTimeDiff.end()){
					vector<double> h  = minTimeDiff[make_pair(itr4->second[i],itr4->first)];
				//	cout<<itr4->second[i]<<"-"<<itr4->first<<":"<<h[0]<<"\t"<<h[1]<<"\t"<<h[2]<<"\t"<<h[3]<<"\t"<<h[4]<<"\t"<<h[5]<<"\t"<<h[6]<<"\t"<<h[7]<<"\t"<<h[8]<<"\t"<<h[9]<<endl;
					itr4->second[i+3] = to_string(h[1]);
					itr4->second[i+4] = to_string(h[2]);
					itr4->second[i+5] = to_string(h[3]);
					itr4->second[i+6] = to_string(h[4]);
					itr4->second[i+7] = to_string(h[5]);
					itr4->second[i+8] = to_string(h[6]);
					itr4->second[i+9] = to_string(h[7]);
					itr4->second[i+10] = to_string(h[8]);
					itr4->second[i+11] = to_string(h[9]);
				//	cout<<itr4->second[i]<<"-"<<itr4->first<<":"<<itr4->second[i+3]<<"\t"<<itr4->second[i+4]<<"\t"<<itr4->second[i+5]<<"\t"<<itr4->second[i+6]<<"\t"<<itr4->second[i+7]<<"\t"<<itr4->second[i+8]<<"\t"<<itr4->second[i+9]<<"\t"<<itr4->second[i+10]<<"\t"<<itr4->second[i+11]<<endl;
					
				//	cout <<itr4->second[i]<<"\t"<<itr4->second[i+1]<<"\t"<<itr4->second[i+2]<<"\t"<<itr4->second[i+3]<<"\t"<<h[0]<<"\t"<<h[1]<<"\t"<<h[2]<<"\t"<<h[3]<<";\t";
				}
				else {
					//cout <<itr4->second[i]<<"\t"<<itr4->second[i+1]<<"\t"<<itr4->second[i+2]<<"\t"<<itr4->second[i+3]<<";\t";
				}
				
			}
		}
	//	cout<<endl;
		influenceOrder.pop();
	}
	//opFile.close();

	return cascade;

}


void maxSpanningCascadeTree(map<string,vector<string> > c, char* g){
	map<string,vector<string> >::iterator citr;
	
	float max_edge = 0;
	string influencer= "";
	string influenced = "";
	
	/* For time only max spanning tree */
	ofstream treeTOpFile(string(g)+"_time_MST.csv");
	treeTOpFile<<"Influencer,Influenced,edgeWt\n";
	
	for (citr = c.begin(); citr != c.end(); ++citr){
		influencer = "";
		max_edge = 0;
		influenced = citr->first;
		//cout<<influenced<<endl;
		for (int i = 1; i < citr->second.size(); i+=12){
			if (citr->second[i].compare("NULL") != 0){
				if (stof(citr->second[i+5]) > max_edge){
					max_edge = stof(citr->second[i+5]);
					influencer = citr->second[i];
				}
			}
			
		}
		if (!influencer.empty()){
			treeTOpFile<<influencer<<","<<influenced<<","<<max_edge<<endl;
			//cout<<influencer<<","<<influenced<<","<<max_edge<<endl;
		}
	}
	treeTOpFile.close();
	
	
	/* Distance based max spanning tree. */
	ofstream treeDOpFile(string(g)+"_dist_MST.csv");
	treeDOpFile<<"Influencer,Influenced,edgeWt\n";

	for (citr = c.begin(); citr != c.end(); ++citr){
		influencer = "";
		max_edge = 0;
		influenced = citr->first;
		for (int i = 1; i < citr->second.size(); i+=12){
			if (citr->second[i].compare("NULL") != 0){
				if (stof(citr->second[i+8]) > max_edge){
					max_edge = stof(citr->second[i+8]);
					influencer = citr->second[i];
				}
			}
			
		}
		if (!influencer.empty()){
			treeDOpFile<<influencer<<","<<influenced<<","<<max_edge<<endl;
			//cout<<influencer<<","<<influenced<<","<<max_edge<<endl;
		}
	}
	
	treeDOpFile.close();
	
	
	
	
	/* Time and distance based max spanning tree. */
	ofstream treeTDOpFile(string(g)+"_time_dist_MST.csv");
	treeTDOpFile<<"Influencer,Influenced,edgeWt\n";

	for (citr = c.begin(); citr != c.end(); ++citr){
		influencer = "";
		max_edge = 0;
		influenced = citr->first;
	//	cout<<influenced<<"\t";
		for (int i = 1; i < citr->second.size(); i+=12){
			if (citr->second[i].compare("NULL") != 0){
				//cout<<citr->second[i]<<":"<<citr->second[i+11]<<endl;
				if (stof(citr->second[i+11]) > max_edge){
					max_edge = stof(citr->second[i+11]);
					influencer = citr->second[i];
				}
			}
			
		}
		if (!influencer.empty()){
			treeTDOpFile<<influencer<<","<<influenced<<","<<max_edge<<endl;
		//	cout<<influencer<<","<<influenced<<","<<max_edge<<endl;
		}
	}
	
	treeTDOpFile.close();
	
	
}

void maxlikelihoodInf(map<string,vector<string> > c, char* g, int k){
	
	map<string,vector<string> >::iterator citr;
	multimap<float, pair<string, string> > sortedEdges;
	multimap<float, pair<string, string> >::reverse_iterator eitr;
	
	float edgeWt = 0;
	string influencer= "";
	string influenced = "";
	
	
	for (citr = c.begin(); citr != c.end(); ++citr){
		influencer = "";
		edgeWt = 0;
		influenced = citr->first;
		for (int i = 1; i < citr->second.size()-5; i+=6){
			if (citr->second[i].compare("NULL") != 0){
				if (stof(citr->second[i+3]) > 0){
					edgeWt = stof(citr->second[i+3]);
					influencer = citr->second[i];
					sortedEdges.insert(make_pair(edgeWt,make_pair(influencer,influenced)));
				}
			}
			
		}
	}
	
	/* Maxliklihood graph: time only.*/
	ofstream maxlikelihoodTInfF (string(g)+"_time_MLG.csv");
	maxlikelihoodTInfF<<"Influencer,Influenced,edgeWt\n";

	for (eitr = sortedEdges.rbegin(); eitr != sortedEdges.rend(); ++eitr){
		//cout<<eitr->first<<","<<eitr->second.first<<","<<eitr->second.second<<"\n";
		maxlikelihoodTInfF<<eitr->second.first<<","<<eitr->second.second<<","<<eitr->first<<endl;
	}
	
	maxlikelihoodTInfF.close();
	
	/* Maxlikelihood graph: time and distance */
	
	sortedEdges.clear();
	
	for (citr = c.begin(); citr != c.end(); ++citr){
		influencer = "";
		edgeWt = 0;
		influenced = citr->first;
		for (int i = 1; i < citr->second.size()-5; i+=6){
			if (citr->second[i].compare("NULL") != 0){
				if (stof(citr->second[i+5]) > 0){
					edgeWt = stof(citr->second[i+5]);
					influencer = citr->second[i];
					sortedEdges.insert(make_pair(edgeWt,make_pair(influencer,influenced)));
				}
			}
			
		}
	}
	
	
	ofstream maxlikelihoodTDInfF (string(g)+"_time_dist__MLG.csv");
	maxlikelihoodTInfF<<"Influencer,Influenced,edgeWt\n";
	for (eitr = sortedEdges.rbegin(); eitr != sortedEdges.rend(); ++eitr){
		//cout<<eitr->first<<","<<eitr->second.first<<","<<eitr->second.second<<"\n";
		maxlikelihoodTDInfF<<eitr->second.first<<","<<eitr->second.second<<","<<eitr->first<<endl;
	}
	
	maxlikelihoodTDInfF.close();
	
	
}


int main(int argc, char **argv)
{

	char* graphemeFile = argv[1]; //graphemes file. Format: 
	char* geoFile = argv[2];      //sites with geo info file. Format: 
	char* grapheme = argv[3];    // the identifier for the grapheme whose influence graph is to be built
	//char* k = argv[4];
	//	char* method = argv[5]; // 0 = temporal influence, 1 temporal + geo distance, 2 = temporal + geodistance + metadata
	
	
	/*read grapheme inscription data along with frequencies of inscriptions*/
	map<string,map<int, map<string,int> > > grapheme_date_site_freq_record = cascadesAllDatesFreq(graphemeFile,grapheme);

	
	/*read site to site straight line distance data*/
	map<pair<string,string>,double> geoData = readGeoDist(geoFile); 	
	
			
	/*build cascade with distance and other metadata such as frequency of inscription. */
	map<string,vector<string> > cascadeDAG = buildCascade_metaData(grapheme_date_site_freq_record,geoData,grapheme);
	
	/*build Max Spanning Tree of the cascade. (= 1 connected component with all the inscribing sites present)*/
	maxSpanningCascadeTree(cascadeDAG, grapheme);
	
	/*build Maxlikelihood graph based on the highest weighted k edges.  (= graph not tree. sites can have multiple sources. Multi components. Not all sites maybe present.)*/
//	maxlikelihoodInf(cascadeDAG, grapheme, atoi(k));

	
	return 0;
}
