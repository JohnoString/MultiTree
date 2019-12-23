#ifndef MULTITREE_H_
#define MULTITREE_H_

#include <algorithm>
#include <typeinfo.h>
#include "stdio.h"
#include "Stack.h"

template <typename T> class MultiTree;


///////////////////////////////////////////////////////////////////////////////////////////////
/*���ӽṹ�ͷ���*/
///////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
struct Cleaner
{
	static void clean(T x)
	{ //�൱�ڵݹ��
#ifdef _DEBUG
		static int n = 0;
		if (7 > strlen(typeid (T).name()))
		{ //��������һ�ź��ԣ�ֻ�����������
			printf("\t<%s>[%d]=", typeid (T).name(), ++n);
			//print(x);
			printf(" purged\n");
		}
#endif
	}
};

template <typename T>
struct Cleaner<T*>
{
	static void clean(T* x)
	{
		if (x)
		{
			delete x;  //������а���ָ�룬�ݹ��ͷ�
		}
#ifdef _DEBUG                
		static int n = 0;
		printf("\t<%s>[%d] released\n", typeid (T*).name(), ++n);
#endif
	}

};

template <typename T>
void release(T x)
{
	Cleaner<T>::clean(x);
}


///////////////////////////////////////////////////////////////////////////////////////////////
/*�ڵ�ģ����*/
///////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
class TreeNode
{
public:
	friend class MultiTree<T>;


	// ��Ա
	T data; //����

	// ���캯��
	TreeNode() :
		_parent(NULL), elder(NULL), younger(NULL), child(NULL) { }

	TreeNode(T d, TreeNode<T>* p = NULL, TreeNode<T>* eld = NULL, TreeNode<T>* young = NULL, TreeNode<T>* ch = NULL) :
		data(d), _parent(p), elder(eld), younger(young), child(ch) { }

	TreeNode<T>* parent() const
	{
		return this->_parent;
	}

	int depth() const //���ڵ�NULL���Լ��ľ���
	{
		int n = 0;
		const TreeNode<T>* p = this;
		while (p)
		{
			n++;
			p = p->_parent;
		}
		return n;
	}

private:
	// ��Ա
	TreeNode<T>* _parent; //���ڵ�
	TreeNode<T>* elder; //�ֳ�
	TreeNode<T>* younger; //�ܵ�
	TreeNode<T>* child; //��С����


	// �����ӿ�
	TreeNode<T>* insertAsCh(T const&); //�½��ڵ���Ϊ��ǰ�ڵ����С���Ӳ���
	template <typename VST> void travPre(VST&); //�����������
	void removeSucc(); //�ݹ�ɾ��x���ڵ㼰����
};

///////////////////////////////////////////////////////////////////////////////////////////////
/*�ڵ�ģ����ʵ��*/
///////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
TreeNode<T>* TreeNode<T>::insertAsCh(T const& d)  //�½��ڵ���Ϊ��ǰ�ڵ����С���Ӳ���
{
	TreeNode<T>* ch = new TreeNode(d, this, this->child);
	if (this->child)
	{
		this->child->younger = ch;
	}
	this->child = ch;
	return ch;
}

template<typename T>
template<typename VST>
void TreeNode<T>::travPre(VST& visit)  //�����������
{
	Stack<TreeNode<T>*> S; //����ջ
	TreeNode<T>* x = this;
	TreeNode<T>* temp;
	S.push(x);  //���ڵ���ջ
	while (!S.empty())  //��ջ���֮ǰ����ѭ��
	{
		x = S.pop();
		visit(x->data);  //���������ʵ�ǰ�ڵ㣬��ǿպ��ӵ���ջ����Ϊ���Һ���
		temp = x->child;
		while (temp)
		{
			S.push(temp);
			temp = temp->elder;
		}
	}
}

template <typename T> //�ݹ�ɾ��x���ڵ㼰����
void TreeNode<T>::removeSucc()
{
	TreeNode<T>* x = this;
	if (x == NULL)
		return; //�ݹ��
	TreeNode<T>* temp;
	while (x->child)
	{
		temp = x->child;
		x->child = temp->elder;
		temp->removeSucc();
	}
	release(x->data); //release()�����ͷŸ��ӽṹ
	release(x);
}

///////////////////////////////////////////////////////////////////////////////////////////////
/*�����ģ����*/
///////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
class MultiTree
{
protected:
	int _size; //��ģ
	int _height; //����
	TreeNode<T>* _root; //���ڵ�ָ��

public:
	MultiTree() : _size(0), _height(0), _root(NULL) { } //���캯��

	virtual ~MultiTree() //��������
	{
		if (0 < _size)
			remove(_root);
	}

	bool empty() const  //�п�
	{
		return !_root;
	}

	TreeNode<T>* root() const //����
	{
		return _root;
	}

	int size() const  //��ģ
	{
		return _size;
	}

	int height() const  //��ģ
	{
		return _height;
	}

	TreeNode<T>* insertAsRoot(T const& e); //������ڵ�

	TreeNode<T>* insertAsYC(TreeNode<T>* x, T const& e); //e��Ϊx����С���ӣ�ԭ�ޣ�����

	void remove(TreeNode<T>* x); //ɾ����λ��x���ڵ�Ϊ��������

	void travPre(void(*visit) (T&)) //��������ָ�����
	{
		if (_root)
			_root->travPre(visit);
	} //��������

	template <typename VST> //�ò�����VST����
	void travPre(VST& visit)
	{
		if (_root)
			_root->travPre(visit);
	} //�������

};

///////////////////////////////////////////////////////////////////////////////////////////////
/*�����ģ��ʵ��*/
///////////////////////////////////////////////////////////////////////////////////////////////
template <typename T> //ɾ����������λ��x���Ľڵ㼰����
void MultiTree<T>::remove(TreeNode<T>* x)
{ //assert: xΪ�������еĺϷ�λ��
	if (x->_parent && x->_parent->child == x)
	{
		x->_parent->child = (x->elder) ? x->elder : NULL; //�ж����Ը��ڵ��ָ��
	}
	if (x->younger)
	{
		x->younger->elder = x->elder; //�ж����Եܵܵ�ָ��
	}
	if (x->elder)
	{
		x->elder->younger = x->younger; //�ж������ֳ���ָ��
	}
	x->removeSucc();
}

template<typename T>
TreeNode<T>* MultiTree<T>::insertAsRoot(T const& e)
{
	_size = _height = 1;
	return _root = new TreeNode<T>(e);
}

template <typename T>
TreeNode<T>* MultiTree<T>::insertAsYC(TreeNode<T>* x, T const& e)  //e����Ϊx����С����
{
	_size++;
	_height = _height > x->depth() ? _height : x->depth();
	x->insertAsCh(e);
	return x->child;
}


#endif // MULTITREE_H_
