#include"Graph.h"

void Graph:: updateRootNode(){
	rootNode = &nodes.find(rootId)->second;
}

void Graph:: updateDescendantNode(){

	map<string,Node>::iterator it;
	string temp;
	for(it = nodes.begin(); it != nodes.end(); it++) {
		vector<Node*>tmp;
		vector<string> desc = it->second.descendants;
		for(vector<string>::iterator it2 = desc.begin(); it2 != desc.end();it2++)
		{
			temp = nodes.find(*it2)->first;
			tmp.push_back(&nodes.find((*it2))->second);
		}
		it->second.descendantNode = tmp;
	}

}

void Graph::setWind(int wind){

	map<string,Node>::iterator it;

	for(it=nodes.begin(); it!=nodes.end(); it++){
		(*it).second.wind=wind;
	}
}