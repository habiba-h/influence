========================================================================
    Inference (of diffusion paths)
========================================================================

Background of this study:
Classic Maya hieroglyphic writing is one of the best-documented and thoroughly deciphered scripts in prehispanic Mesoamerica, yet we still know remarkably little about how this writing system evolved. Specifically, when and how did novel graphemes -- the discrete and most basic units of text -- spread across the Maya region? A major challenge to inferring influence is that who--influence--whom network is largely an intangible and unobservable phenomenon. Moreover, the underlying network of interactions through which influence potentially propagates is, at best, partially observable in most cases. Taking into consideration these challenges, we devise a probabilistic framework for building networks of grapheme innovation using the observed dates of grapheme inscription and the geographic positions of archaeological sites. We use the Susceptible-Infected (SI) transmission model from mathematical epidemiology as the basis of our proposed method. The inferred influence networks of grapheme innovation are validated  by comparing them with the sociopolitical ties documented in the written texts. These  influence networks facilitate the identification of sites that stood out as innovators at various points during the Classic period. The proposed probabilistic model is applicable to a wide range of archaeological network inference problems and opens up further questions about co-evolution, replacements, and the discontinuation of graphemes through time.


========================================================================

Influence propagation framework: 

Understanding the mechanism by which a set of entities in a social, political, familial environment influence each other has fascinated  researchers in a wide range of scientific arenas for decades now. Although it is natural to assume that individuals influence each other in various forms, the phenomenon is largely unobservable. Moreover, we usually have only partial observation of the network of interactions through which people connect and communicate.  However, what can usually be observed is when(the time) a node adopts a concept. 

Here we address the task of building a temporal stochastic propagation framework by using the observations of when a node “adopts”/inscribes a concept. The overall mechanism of the influence propagation model is based on the  Independent Cascade Model.  Each node is in one of the two states: influenced or susceptible. Each influenced node can influence each susceptible node in future. Once a node is influenced it remains in that state indefinitely.  Each influence is independent of all others.

For more information about the background, motivation, and methodology see:
  	“Networks of Diffusion and Centers of Scribal Innovation in Classic Maya Society”
	http://algo.uni-konstanz.de/members/habiba/papers/Maya_inference_influence_propagation_CAA2017.pdf
  
========================================================================

Parameters:

	temporal information of when a site inscribed a grapheme (example:'example_inscriptions.csv')
	pairwise physical distances of sites (example:'example_geo_distance.csv')
	grapheme code (example: use one of the following HE7, MZ3, 1B1s, HH1)
	    

========================================================================

Usage:
Infer the Maximum Spanning Tree of influence paths for a given set of nodes(sites) that share similar writing style  (nodes and timestamps):

========================================================================

Compiling the code:

Running the code:
./inference example_inscriptions.csv example_geo_distance.csv 1B1s

========================================================================

Format input:

Temporal cascades: (example: 'example_inscriptions.csv')

The temporal cascades are input as a 3 column csv file. 
- First column: the concept that is inscribed/recorded/written.
- Second column: the node/site that records the concept
- Third column: the time at which the inscription of the concept happened at the site

The distances are input as a 2 column csv file.
- First column: node ID 1
- Second column: node ID 2

<concept ID>: The method needs the concept for which the propagation tree is inferred.  This can be any alphanumeric code key from the first column of the example cascade file.

Note: the node IDS have to match the IDs of nodes in the temporal cascades file.

Also, note: the current method infers a tree for one concept at a time. It can naturally be extended to infer paths of propagation for all or subset of graphemes. Since during the current iteration we are interested in understanding the patterns of diffusion for a specific sample of concepts so the code is customized to take one concept at a time. 

========================================================================

Format output:

Maximum spanning tree for the “time model”.
Maximum spanning tree for the “distance model”.
Maximum spanning tree for the “time + distance model”.  (It is assumed that time and distance are mutually independent explanatory variables for the propagation of the concepts). 
Complete list of weighted Directed Acyclic Graph edges for the inscriptions of a grapheme. The weights are calculated based on the model of diffusion described in the link above.
Frequency of inscription of each grapheme along with who inscribed it.

========================================================================
Additional files:

Ground truth:
The inferred paths can be validated with the help of ground truth (if available). For example, if "real" relationships among the set of nodes is partially known, one can verify how well the influence propagation paths can correctly identify those relationships.

Ground truth: 2 column comma separated file.
- Column 1: Node ID
- Column 2: Node ID

Calculate geo distance among the nodes. 

This code simply calculates the "Havershine" distance between two points on earth based on their latitudes and longitudes. 

Note: instead of distance any other pairwise relationship between a pair of nodes can be used.

This inference method uses proximity in time and distance to measure the relative influence of one node over another using the exponential decay model. This method can be modified to incorporate other metadata to improve the inference procedure. 
