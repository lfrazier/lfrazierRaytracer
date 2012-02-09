#include "SceneGraph.h"

SceneGraph::SceneGraph(int x, int z, int numItems){
	root = new Node();
	root->transform = glm::mat4(1.0);
	// Change this later
	root->furn = new Floor(x, z);
	root->mesh = NULL;
	root->yDisp = -1;
	preorder = root->preorderList(vector<Node *>());
}

void SceneGraph::addNode(Node *node) {
	root->addChild(node);
}

void SceneGraph::updatePreorder() {
	preorder = root->preorderList(vector<Node *>());
}

Node * SceneGraph::removeNode(Node *node) {
	if (node == root) {
		return root;
	}
	root->removeNode(node);
	updatePreorder();
	return root;
}