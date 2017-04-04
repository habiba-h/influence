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



map<pair<string,string>,double> readAllCascades(char* fileName){
	
	
	ifstream geoFile(fileName);
	map<pair<string,string>,double> influence_graph;
	
	
	map<string, int> cascades_sites_edges;
	
	if (!geoFile) cout<<"File error!!\t"<<fileName<<endl;
	else{
			string line;
			string siteX,siteY;
			double influence_wt;
			int wordCount = 0;
			string word;
			getline(geoFile,line); //header
			cout<<line<<endl;
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
						influence_wt = stod(word);
						wordCount++;
					}
					else break;
				}
				
			
				if (cascades_sites_edges.find(siteX) != cascades_sites_edges.end()){
					cascades_sites_edges[siteX] +=1; 
				}
				else cascades_sites_edges[siteX] = 1;
				
				
				if (cascades_sites_edges.find(siteY) != cascades_sites_edges.end()){
					cascades_sites_edges[siteY] +=1; 
				}
				else cascades_sites_edges[siteY] = 1;
				
				
				if (influence_graph.find(make_pair(siteX,siteY)) != influence_graph.end()){
					influence_graph[make_pair(siteX,siteY)] += influence_wt;
				}
				else if (influence_graph.find(make_pair(siteY,siteX)) != influence_graph.end()){
					influence_graph[make_pair(siteY,siteX)] += influence_wt;
				}
				else
					influence_graph.insert(make_pair(make_pair(siteX,siteY),influence_wt));
			}
		
	}
	
	map<string, int>::iterator itr;
	
	int allinteractions = 0;
	
	for (itr = cascades_sites_edges.begin(); itr!= cascades_sites_edges.end(); ++itr){
		cout<<itr->first<<"\t"<<itr->second<<endl;
		allinteractions += itr->second;
	}
	
	cout<<cascades_sites_edges.size()<<endl;
	
	
	
	return influence_graph;
}


map<pair<string,string>,int> readAllRelationships(char* fileName){
	
	ifstream relationshipFile(fileName);
	
	map<pair<string, string>, int > relations;
	map<pair<string, string>, int >::iterator rel;
	
	map<pair<string, string>, map<string, int> > relations_types;
	
	if (!relationshipFile) cout<<"File error!!\t"<<fileName<<endl;
	else{
		int dyadCount  = 0;
		map<string, int> m;
		string relationshipType = "";
		string line = "";
		while (getline(relationshipFile, line))
		{	
		//	cout<<line<<endl;
			++dyadCount;
			int wordcount = 0;
			stringstream strstr(line);
			string word = "";
			string from = "";
			string to = "";	
			
			while (getline(strstr,word, ','))
			{		
				++wordcount;
				if (wordcount == 1)
					from = word;
				else if (wordcount == 2)
					to = word;
				else {
						if (wordcount % 2 != 0)
							relationshipType = word;
						else 
							m[relationshipType] = stoi(word);
					}		
				}
				pair<string, string> p(from, to);		
				pair<string, string> p1(to, from);
				if ((relations.find(p) == relations.end()) && (relations.find(p1) == relations.end())){
					relations[p] = 1;
					cout<<line<<endl;
				}
				
				relations_types[p] = m;
				m.clear();
				wordcount = 0;
				from = "";
				to = "";
			}
		}
	
	return relations;
}

void compare_graphs(map<pair<string, string>, double> graphemes,map<pair<string, string>, int> relations){
	
	string opFile = "relations_inference_comparison.csv";
	ofstream comparisonF(opFile);
	
	map<string, int>	inferred_edges;
	map<string, int> relationship_edges;
		
	map<pair<string, string>, double>::iterator itr1;
	map<pair<string, string>, int>::iterator itr2;
	
	comparisonF<<"siteX,siteY,infWt,presence\n";
	for (itr1 = graphemes.begin(); itr1 != graphemes.end(); ++itr1){
		/*if (relations.find(itr1->first) != relations.end()){
			cout<<itr1->first.first<<","<<itr1->first.second<<","<<itr1->second<<","<<0<<endl;
			comparisonF<<itr1->first.first<<","<<itr1->first.second<<","<<itr1->second<<","<<0<<endl;
		}
		*/
		if (relations.find(make_pair(itr1->first.first,itr1->first.second)) != relations.end()){
			cout<<itr1->first.first<<","<<itr1->first.second<<","<<itr1->second<<","<<0<<endl;
			comparisonF<<itr1->first.first<<","<<itr1->first.second<<","<<itr1->second<<","<<0<<endl;
		}
		else if (relations.find(make_pair(itr1->first.second,itr1->first.first)) != relations.end()){
			cout<<itr1->first.second<<","<<itr1->first.first<<","<<itr1->second<<","<<10<<endl;
			comparisonF<<itr1->first.second<<","<<itr1->first.first<<","<<itr1->second<<","<<10<<endl;
		}
		
	
	}
	
	
	for (itr2 = relations.begin(); itr2 != relations.end(); ++itr2){
		if (graphemes.find(make_pair(itr2->first.first,itr2->first.second)) != graphemes.end()){
			cout<<itr2->first.first<<","<<itr2->first.second<<","<<itr2->second<<","<<01<<endl;
			comparisonF<<itr2->first.first<<","<<itr2->first.second<<","<<itr2->second<<","<<01<<endl;
		}
		else if (graphemes.find(make_pair(itr2->first.second,itr2->first.first)) != graphemes.end()){
			cout<<itr2->first.first<<","<<itr2->first.second<<","<<itr2->second<<","<<11<<endl;
			comparisonF<<itr2->first.first<<","<<itr2->first.second<<","<<itr2->second<<","<<11<<endl;
		}
		
	
	}
	
	
/*	
	for (itr2 = relations.begin(); itr2 != relations.end(); ++itr2){
		if (graphemes.find(itr2->first) == graphemes.end()){
			cout<<itr2->first.first<<","<<itr2->first.second<<",0,"<<1<<endl;
			comparisonF<<itr2->first.first<<","<<itr2->first.second<<",0,"<<1<<endl;
		}
		
	}
	
	for (itr1 = graphemes.begin(); itr1 != graphemes.end(); ++itr1){
		if (relations.find(itr1->first) == relations.end()){
			cout<<itr1->first.first<<","<<itr1->first.second<<","<<itr1->second<<","<<2<<endl;
			comparisonF<<itr1->first.first<<","<<itr1->first.second<<","<<itr1->second<<","<<2<<endl;
		}
		
	}
*/	
	
	comparisonF.close();
	
/*	
	for (itr1 = graphemes.begin(); itr1 != graphemes.end(); ++itr1){
		
		if (inferred_edges.find(itr1->first.first) != inferred_edges.end()){
			inferred_edges[itr1->first.first] += 1;
		}
		else
			inferred_edges[itr1->first.first] = 1;
			
			
		if (inferred_edges.find(itr1->first.second) != inferred_edges.end()){
			inferred_edges[itr1->first.second] += 1;
		}
		else
			inferred_edges[itr1->first.second] = 1;	
	}
	
	
	for (itr2 = relations.begin(); itr2 != relations.end(); ++itr2){
		
		if (relationship_edges.find(itr2->first.first) != relationship_edges.end()){
			relationship_edges[itr2->first.first] += 1;
		}
		else
			relationship_edges[itr2->first.first] = 1;
			
			
		if (relationship_edges.find(itr2->first.second) != relationship_edges.end()){
			relationship_edges[itr2->first.second] += 1;
		}
		else
			relationship_edges[itr2->first.second] = 1;	
	}
	
	
	
	map<string, int>::iterator itr;
	
	
	int allinteractions = 0;
	
	for (itr = relationship_edges.begin(); itr!= relationship_edges.end(); ++itr){
		cout<<itr->first<<"\t"<<itr->second<<endl;
		allinteractions += itr->second;
	}
	
	float densityRelations = allinteractions/(relationship_edges.size()*(relationship_edges.size()-1));
	cout<<relationship_edges.size()<<"\t"<<allinteractions/2<<"\t"<<densityRelations<<endl;
	
	allinteractions = 0;
	for (itr = inferred_edges.begin(); itr!= inferred_edges.end(); ++itr){
		cout<<itr->first<<"\t"<<itr->second<<endl;
		allinteractions += itr->second;
	}
	
	float densityinferred = allinteractions/(inferred_edges.size()*(inferred_edges.size()-1));
	cout<<inferred_edges.size()<<"\t"<<allinteractions/2<<"\t"<<densityinferred<<endl;
	
	int matches = 0;
	int mismatches = 0;
	map<string, int> matchedSites;
	
	for (itr = relationship_edges.begin(); itr!= relationship_edges.end(); ++itr){
		if (inferred_edges.find(itr->first) != inferred_edges.end()){
			++matches;
			cout<<"+ "<<itr->first<<endl;
			matchedSites[itr->first] = 1;
		}
		else{
			++mismatches;
			cout<<"- "<<itr->first<<endl;
		}
		
	}
	
	cout<<"matched nodes:"<<matches<<"unmatched nodes: "<<mismatches<<endl;
	
	
	for (itr2 = relations.begin(); itr2 != relations.end(); ++itr2){
		if ( (matchedSites.find(itr2->first.first) != matchedSites.end()) && (matchedSites.find(itr2->first.second) != matchedSites.end())){
			if (graphemes.find(itr2->first) != graphemes.end()){
				cout <<itr2->first.first<<","<<itr2->first.second<<","<<graphemes[itr2->first]<<","<<0<<endl;
			}
			else{
				cout <<itr2->first.first <<","<<itr2->first.second<<","<<0<<","<<1<<endl;
			}
		}
		else{
			cout <<itr2->first.first <<","<<itr2->first.second<<","<<0<<","<<2<<endl;
		}
		
	}
	
	
	for (itr1 = graphemes.begin(); itr1 != graphemes.end(); ++itr1){
		if ((matchedSites.find(itr1->first.first) != matchedSites.end()) && (matchedSites.find(itr1->first.second) != matchedSites.end())){
			cout <<itr1->first.first <<" + "<<itr1->first.second<<" "<<itr1->second<<endl;
		}
		else{
			cout <<itr1->first.first <<" x "<<itr1->first.second<<" "<<itr1->second<<endl;
		}
	}
	
	for (itr1 = graphemes.begin(); itr1 != graphemes.end(); ++itr1){
	
			cout <<itr1->first.first <<" * "<<itr1->first.second<<" "<<itr1->second<<endl;
	
		
	}
*/	
	
}


int main(int argc, char **argv)
{

	
	
	char* graphemeTreesFile = argv[1]; // all inferred cascades
	char* relationshipFile = argv[2]; // all relationships
	
	
	
	/*read all grapheme cascades*/
	map<pair<string, string>, double> grapheme_cascades_graph = readAllCascades(graphemeTreesFile);

	
	/* *******  test the grapheme graph */
	map<pair<string, string>, double>::iterator itr;
	
	for (itr = grapheme_cascades_graph.begin(); itr != grapheme_cascades_graph.end(); ++itr){
		cout<<itr->first.first<<" "<<itr->first.second<<" "<<itr->second<<endl;
	}



	map<pair<string, string>, int> relationship_graph = readAllRelationships(relationshipFile);
	
	
	/* *******  test the relationship graph */
	/*map<pair<string, string>, int>::iterator itr1;
	
	for (itr1 = relationship_graph.begin(); itr1 != relationship_graph.end(); ++itr1){
		cout<<itr1->first.first<<" "<<itr1->first.second<<" "<<itr1->second<<endl;
	}
	*/

	compare_graphs(grapheme_cascades_graph,relationship_graph);

	
	return 0;
}
