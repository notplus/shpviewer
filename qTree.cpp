#include "qTree.h"

void Region::operator=(const Region&b)
{
	up = b.up;
	bottom = b.bottom;
	left = b.left;
	right = b.right;
}

bool Region::operator==(const Region&b)
{
	if (fabs(bottom - b.bottom) < 1e-6 && fabs(up - b.up) < 1e-6
		&& fabs(left - b.left) < 1e-6 && fabs(right - b.right) < 1e-6)
		return true;
	else
		return false;
}

bool Region::operator<(const Region& b)
{
	if (bottom > b.bottom && up < b.up && left > b.left && right < b.right )
		return true;
	else
		return false;
}

bool Region::operator<=(const Region&b)
{
	//if (*this < b || *this == b)
	if ((bottom>b.bottom || fabs(bottom-b.bottom)<1e-6) 
		&& (up<b.up || fabs(up-b.up)<1e-6)
		&& (left>b.left || fabs(left-b.left)<1e-6)
		&& (right<b.right || fabs(right-b.right)<1e-6))
		return true;
	else
		return false;
}

void initRegion(Region *region, double up, double bottom, double left, double right)
{
	region->bottom = bottom;
	region->left = left;
	region->right = right;
	region->up = up;
}

void qTree::initNode(qTree* node, int depth, Region region)
{
	node->depth = depth;
	node->is_leaf = true;
	node->ele_num = 0;
	node->region = region;
}

qTree creatRoot()
{
	qTree tree;
	Region rootRegion;
	initRegion(&rootRegion, 90, -90, -180, 180);
	tree.initNode(&tree, 1, rootRegion);
	return tree;
}

void qTree::insertEle(qTree* node, elePoint ele)
{
	if (node->is_leaf)
	{
		if (node->ele_num + 1 > MAX_ELE_NUM)
		{
			splitNode(node);
			insertEle(node, ele);
		}
		else
		{
			elePoint* ele_ptr = (elePoint*)(malloc(sizeof(elePoint)));
			node->ele_list[node->ele_num] = (element*)(malloc(sizeof(element)));
			ele_ptr->x = ele.x;
			ele_ptr->y = ele.y;
			node->ele_list[node->ele_num]->ele_point = *ele_ptr;
			free(ele_ptr);			//may have errors
			node->ele_num++;
		}
		return;
	}

	double midHor = (node->region.up + node->region.bottom) / 2;
	double midVer = (node->region.right + node->region.left) / 2;
	if (ele.x > midHor)
	{
		if (ele.y > midVer)
			insertEle(node->ru, ele);
		else
			insertEle(node->lu, ele);
	}
	else
	{
		if (ele.x > midVer)
			insertEle(node->rb, ele);
		else
			insertEle(node->lb, ele);
	}
}

void qTree::insertEle(qTree* node, eleLine ele)
{
	if (node->is_leaf)
	{
		if (node->ele_num + 1 > MAX_ELE_NUM)
		{
			splitNode(node);
			insertEle(node, ele);
		}
		else
		{
			eleLine *ele_ptr = (eleLine*)(malloc(sizeof(eleLine)));
			node->ele_list[node->ele_num] = (element*)(malloc(sizeof(element)));
			ele_ptr->x1 = ele.x1;
			ele_ptr->x2 = ele.x2;
			ele_ptr->y1 = ele.y1;
			ele_ptr->y2 = ele.y2;
			node->ele_list[node->ele_num]->ele_line = *ele_ptr;
			free(ele_ptr);
			node->ele_num++;
		}
		return;
	}

	double midHor = (node->region.up + node->region.bottom) / 2;
	double midVer = (node->region.right + node->region.left) / 2;
	if (ele.y1 > midHor && ele.y2 > midHor)
	{
		if (ele.x1 > midVer)
			insertEle(node->ru, ele);
		else
			insertEle(node->lu, ele);
	}
	else
	{
		if (ele.x1 > midVer)
			insertEle(node->rb, ele);
		else
			insertEle(node->lb, ele);
	}
}

void qTree::splitNode(qTree *node)
{
	double midHor = (node->region.up + node->region.bottom) / 2;
	double midVer = (node->region.right + node->region.left) / 2;

	node->is_leaf = false;
	node->ru = createChild(node, midHor, node->region.up, midVer, node->region.right);
	node->rb = createChild(node, node->region.bottom, midHor, midVer, node->region.right);
	node->lu = createChild(node, midHor, node->region.up, node->region.left, midVer);
	node->lb = createChild(node, node->region.bottom, midHor, node->region.left, midVer);

	for (size_t i = 0; i < MAX_ELE_NUM; i++)
	{
		if (fabs(node->ele_list[i]->ele_point.x)>10000)
			insertEle(node, node->ele_list[i]->ele_line);
		else
			insertEle(node, node->ele_list[i]->ele_point);
		free(node->ele_list[i]);
		node->ele_num--;
	}
}

qTree* qTree::createChild(qTree *node, double bottom, double up, double left, double right)
{
	int depth = node->depth + 1;
	qTree *childNode = (qTree*)(malloc(sizeof(qTree)));
	Region *region = (Region*)(malloc(sizeof(Region)));
	initRegion(region, up, bottom, left, right);
	initNode(childNode, depth, *region);

	return childNode;
}

void includeMin(qTree* tree, Region* region,Region* min_include)
{
	if (tree->region < *region)
	{
		*min_include = tree->region;
		return;
	}
	if (!tree->is_leaf)
	{
		if (!(*region<tree->ru->region) && !(*region<tree->rb->region) &&
			!(*region<tree->lu->region) && !(*region<tree->lb->region))
		{
			*min_include = tree->region;
			return;
		}	
		else if(*region<tree->lb->region)
			includeMin(tree->lb, region, min_include);
		else if(*region<tree->lu->region)
			includeMin(tree->lu, region, min_include);
		else if(*region<tree->rb->region)
			includeMin(tree->rb, region, min_include);
		else
			includeMin(tree->ru, region, min_include);
	}
	else
	{
		return;
	}
}

void returnTree(qTree* tree, Region* region,qTree* &ret)
{
	if (tree->region == *region)
	{
		ret = tree;
		
		return;
	}
	else
	{
		if (!tree->is_leaf)
		{
			if (*region <= tree->lu->region)
				returnTree(tree->lu, region, ret);
			else if (*region <= tree->lb->region)
				returnTree(tree->lb, region, ret);
			else if (*region <= tree->ru->region)
				returnTree(tree->ru, region, ret);
			else
				returnTree(tree->rb, region, ret);
		}
	}
}