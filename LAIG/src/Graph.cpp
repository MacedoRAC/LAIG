#include"Graph.h"

void Graph:: updateRootNode(){
	rootNode = &nodes.find(rootId)->second;
}

void Graph:: updateDescendantNode(){
	map<string,Node>::iterator it = nodes.begin();
	vector<Node*> temp;
	
	while(it != nodes.end()){
		
		for (unsigned int i=0 ; i<it->second.descendants.size();i++)
			temp.push_back(&nodes.find(it->second.descendants[i])->second);
		
		it->second.descendantNode=temp;

		it++;
	}
}