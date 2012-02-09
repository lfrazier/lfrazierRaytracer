#include "Node.h"
#include <vector>

using namespace std;

class SceneGraph {
public:
	Node *root;
	vector<Node *> preorder;
	SceneGraph(int x, int z, int numItems);
	void addNode(Node *node);
	void updatePreorder();
	Node * removeNode(Node *node);
};