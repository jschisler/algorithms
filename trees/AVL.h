
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
class cUser; // Add a class to store

typedef cUser* element;

struct AVL_Node
{
	int key;
	element data;
	AVL_Node      *parent,
		*left,
		*right;

	AVL_Node()
	{
		left = right = parent = nullptr;
	}

	char factor; //byte
};
//-------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------- s
class AVL_Tree
{
public:
	AVL_Tree();
	void insert(int key);
	void erase(int key);
	element find(int key);
	void dfs();
	int size;
	inline int height(AVL_Node * pos) const;
	AVL_Node *root;

private:
protected:
	AVL_Node *tmp;
	AVL_Node *K1, *K2, *K3, *newnode;
	cUser * tmpdata;
	bool h;

	inline int max(int a, int b) const;

	AVL_Node * singleRotateLeft(AVL_Node *K2);
	AVL_Node * singleRotateRight(AVL_Node *K2);

	AVL_Node * doubleRotateLeft(AVL_Node *K2);
	AVL_Node * doubleRotateRight(AVL_Node *K2);

	AVL_Node * _insert(int key, AVL_Node * node);
	AVL_Node * _erase(int key, AVL_Node * node);
	void _dfs(AVL_Node * v) const;
};