/*$T AVL.cpp GC 1.150 2014-02-27 10:54:55 */

/*$6*/


#include "stdafx.h"
#include <cstdio>
#include <iostream>
#include "Avl.h"

//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
AVL_Tree::AVL_Tree()
{
	size = 0;

	root = nullptr;
}

//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
element AVL_Tree::find( int key )
{
	if ( size == 0 ) return nullptr;
	tmp = root;
	while ( tmp != nullptr )
	{
		if ( key < tmp->key )
			tmp = tmp->left;
		else if ( key > tmp->key )
			tmp = tmp->right;
		else
			return tmp->data;
	}

	return nullptr;
}

//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
inline int AVL_Tree::height( AVL_Node *pos ) const
{
	if ( pos == nullptr )
		return -1;
	else
		return pos->factor;
}

//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
inline int AVL_Tree::max( int a, int b ) const
{
	return a > b ? a : b;
}

//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
AVL_Node *AVL_Tree::singleRotateLeft( AVL_Node *K2 )
{
	K1 = K2->left;
	K2->left = K1->right;
	K1->right = K2;

	K2->factor = max( height(K2->left), height(K2->right) ) + 1;
	K1->factor = max( height(K1->left), K2->factor ) + 1;

	return K1;	// new root
}

//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
AVL_Node *AVL_Tree::singleRotateRight( AVL_Node *K1 )
{
	K2 = K1->right;
	K1->right = K2->left;
	K2->left = K1;

	K1->factor = max( height(K1->left), height(K1->right) ) + 1;
	K2->factor = max( height(K2->right), K1->factor ) + 1;

	return K2;	// new root
}

//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
AVL_Node *AVL_Tree::doubleRotateLeft( AVL_Node *K3 )
{
	K3->left = singleRotateRight( K3->left );

	return singleRotateLeft( K3 );
}

//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
AVL_Node *AVL_Tree::doubleRotateRight( AVL_Node *K1 )
{
	K1->right = singleRotateLeft( K1->right );

	return singleRotateRight( K1 );
}

//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
void AVL_Tree::insert( int key )
{
	size++;
	root = _insert( key, root );
}

//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
AVL_Node *AVL_Tree::_insert( int key, AVL_Node *node )
{
	if ( node == nullptr )
	{
		node = new AVL_Node;
		node->factor = 0;
		node->key = key;
		node->data = tmpdata;
		node->left = nullptr;
		node->right = nullptr;
	}
	else if ( key < node->key )
	{
		node->left = _insert( key, node->left );
		if ( height(node->left) - height(node->right) == 2 )
		{
			if ( key < node->left->key )
				node = singleRotateLeft( node );
			else
				node = doubleRotateLeft( node );
		}
	}
	else if ( key > node->key )
	{
		node->right = _insert( key, node->right );
		if ( height(node->right) - height(node->left) == 2 )
		{
			if ( key > node->right->key )
				node = singleRotateRight( node );
			else
				node = doubleRotateRight( node );
		}
	}

	node->factor = max( height(node->left), height(node->right) ) + 1;
	return node;
}

//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
void AVL_Tree::erase( int key )
{
	root = _erase( key, root );
}

//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
AVL_Node *AVL_Tree::_erase( int key, AVL_Node *node )
{
	bool	done = false;
	if ( node == nullptr )
	{
		h = false;
		done = true;
	}
	else if ( key < node->key )		//delee from left subtree
	{
		newnode = _erase( key, node->left );
		node->left = newnode;
		if ( h )
		{
			if ( height(node->right) - height(node->left) == 2 )
			{
				if ( height(node->right) > height(node->left) )
					node = singleRotateLeft( node );
				else
					node = singleRotateRight( node );
			}

			node->factor = max( height(node->left), height(node->right) ) + 1;

			//          if (node->factor >= 0)
			//          {
			//				node->factor=root->factor-1;
			//              
			//				if (node->factor==-1)
			//					h=false;
			//          }
			//          else if (node->right->factor==-1)
			//				singleRotateRight(node);
			//          else
			//				singleRotateLeft(node);
			done = true;
			return node;
		}
	}
	else if ( key == node->key )	//del node
	{
		if ( node->left == nullptr || node->right == nullptr )	// one or no children
		{
			if ( node->left == nullptr )
				K1 = node->right;
			else
				K1 = node->left;

			delete node;

			h = true;
			done = true;

			return( K1 );
		}
		else	// both of children
		{
			K2 = node->right;
			while ( K2->left != nullptr ) K2 = K2->left;

			node->key = K2->key;
			key = node->key;
		}
	}

	if ( !done && key >= node->key )	// delete from right subtree
	{
		newnode = _erase( key, node->right );
		node->right = newnode;
		if ( h )
		{
			if ( height(node->right) - height(node->left) == 2 )
			{
				if ( height(node->right) > height(node->left) )
					node = singleRotateLeft( node );
				else
					node = singleRotateRight( node );
			}

			node->factor = max( height(node->left), height(node->right) ) + 1;

			//

			/*                if (node->factor <= 0)
			{
			node->factor=node->factor+1;
			if (node->factor ==1)
			h=false;
			}
			else if (node->right->factor==1)
			singleRotateLeft(node);
			else
			singleRotateRight(node);*/
			return node;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
void AVL_Tree::dfs()
{
	_dfs( root );
}

//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
void AVL_Tree::_dfs( AVL_Node *v ) const
{
	if ( v->left != nullptr ) _dfs( v->left );

	std::cout << v->key << std::endl;
	if ( v->right != nullptr ) _dfs( v->right );
}

//-------------------------------------------------------------------------------------------------------------
//---------------------------------
