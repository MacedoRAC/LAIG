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

void Graph:: giveAppearancesToNodes(){

	map<string, Node>::iterator it;
	map<string, Appearance>::iterator itA;

	for(it = nodes.begin(); it != nodes.end(); it++){
		for(itA = appearances.begin(); itA != appearances.end(); itA++){
			if((*it).second.appRef == (*itA).second.id)
				(*it).second.app = &(*itA).second;
			else
				(*it).second.app=NULL; // assegura que se appearance for 'inherit' app é um apontador nulo
		}
	}
}

void Graph::giveTexturesToAppearances(){
	vector<Texture>::iterator itT;
	map<string, Appearance>::iterator itA;

	for(itA = appearances.begin(); itA != appearances.end(); itA++){
		for(itT = textures.begin(); itT != textures.end(); itT++){
			if((*itA).second.textureref == (*itT).id)
				(*itA).second.texture = &(*itT);
			else
				(*itA).second.texture=NULL; // assegura que se nao tiver textura 'texture' é um apontador nulo
		}
	}
}