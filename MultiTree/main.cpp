#include <iostream>
#include "MultiTree.h"
#include <cstdlib>
#include <ctime>

using namespace std;

clock_t start;
clock_t ending;
enum direction { NONE, UP, DOWN, LEFT, RIGHT };

//void test()
//{
//	MultiTree<int> myTree = MultiTree<int>();
//	TreeNode<int>* node[4];
//
//	node[0] = myTree.insertAsRoot(1);
//	node[1] = myTree.insertAsYC(node[0], 2);
//	node[2] = myTree.insertAsYC(node[0], 3);
//	node[3] = myTree.insertAsYC(node[1], 4);
//
//	myTree.travPre([](int & n) { cout << n << " "; });
//}

//������
const int ROW = 4;  //MAXROW
const int COL = 7;  //MAXCOL
const int SROW = 0;  //Started Row
const int SCOL = 3;	 //Started Column
const int TROW = 2;  //Teminateded Row
const int TCOL = 6;  //Teminateded Column
const int INIT[ROW][COL] = { { 0,-1, 0, 1, 0,-1, 0},
							 {-1,-1,-1, 0,-1,-1, 0},
							 {-1, 0,-1,-1,-1,-1, 0},
							 { 0, 0, 0, 0, 0,-1,-1} };

//��ά���ݵ�
class point
{
public:
	int x;
	int y;
	direction dir;

	point(int a = 0, int b = 0, direction d = NONE) :x(a), y(b), dir(d) {};
};


//λ������
point* up(TreeNode<point>* cur, int map[ROW][COL])
{
	int xx = cur->data.x;
	while (xx > -1 && map[xx][cur->data.y] != 0) //��xx���Ϸ�����--
	{
		xx--;
	}
	if (xx == -1 || (xx == TROW && cur->data.y == TCOL))
	{
		return NULL;
	}
	else
	{
		return new point(xx, cur->data.y, UP);
	}
}

point* down(TreeNode<point>* cur, int map[ROW][COL])
{
	int xx = cur->data.x;
	while (xx < ROW && map[xx][cur->data.y] != 0) //��xx���Ϸ�����++
	{
		xx++;
	}
	if (xx == ROW || (xx == TROW && cur->data.y == TCOL))
	{
		return NULL;
	}
	else
	{
		return new point(xx, cur->data.y, DOWN);
	}
}

point* left(TreeNode<point>* cur, int map[ROW][COL])
{
	int yy = cur->data.y;
	while (yy > -1 && map[cur->data.x][yy] != 0) //��yy���Ϸ�����--
	{
		yy--;
	}
	if (yy == -1 || (cur->data.x == TROW && yy == TCOL))
	{
		return NULL;
	}
	else
	{
		return new point(cur->data.x, yy, LEFT);
	}
}

point* right(TreeNode<point>* cur, int map[ROW][COL])
{
	int yy = cur->data.y;
	while (yy < COL && map[cur->data.x][yy] != 0) //��yy���Ϸ�����++
	{
		yy++;
	}
	if (yy == COL || (cur->data.x == TROW && yy == TCOL))
	{
		return NULL;
	}
	else
	{
		return new point(cur->data.x, yy, RIGHT);
	}
}

//�� map ��״̬�ݻ����������Ľڵ� cur ��
int evolution(TreeNode<point>* cur, int map[ROW][COL])
{
	int height = 0;
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			map[i][j] = INIT[i][j];
		}
	}
	TreeNode<point>* temp = cur;
	while (temp)
	{
		height++;
		map[temp->data.x][temp->data.y] = 1;
		temp = temp->parent();
	}
	return height;
}

//���������
int main()
{
	start = clock();  //��ʱ��ʼ
	int map[ROW][COL];  //��¼״̬
	MultiTree<point> searchTree = MultiTree<point>();  //����������
	Stack<TreeNode<point>*> S;  //���帨��ջ
	TreeNode<point>* visit = NULL;  //��ǰ����Ľڵ�
	int depth = 0;  //��ǰ����ڵ����������

	int LENGTH = -1;  //���岢����Ŀ���������
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			if (INIT[i][j] == 0)
			{
				LENGTH++;
			}
		}
	}

	S.push(searchTree.insertAsRoot(point(SROW, SCOL, NONE)));  //���ڵ���ջ
	while (!S.empty() && depth <= LENGTH)  //��δ�ִ�Ŀ�������ջδ��֮ǰ���򷴸�ѭ��
	{
		visit = S.pop(); //������ǰ�ڵ�
		depth = evolution(visit, map);  //�ݻ�����ǰ״̬
		point* pos;  //�ǿպ��ӵ���ջ����Ϊ���Һ���
		if ((visit->data.dir != DOWN) && (pos = up(visit, map)))  //���߻�ͷ·���Ҹ�λ���ǿ��е�
		{
			S.push(searchTree.insertAsYC(visit, *pos));  //������չ��������ѹջ
		}
		if ((visit->data.dir != UP) && (pos = down(visit, map)))
		{
			S.push(searchTree.insertAsYC(visit, *pos));
		}
		if ((visit->data.dir != RIGHT) && (pos = left(visit, map)))
		{
			S.push(searchTree.insertAsYC(visit, *pos));
		}
		if ((visit->data.dir != LEFT) && (pos = right(visit, map)))
		{
			S.push(searchTree.insertAsYC(visit, *pos));
		}
	}

	ending = clock();  //��ʱ����
	double endtime = (double)(ending - start) / CLOCKS_PER_SEC;
	cout << "Total time:" << endtime * 1000 << "ms" << endl;	//����ܺ�ʱ��msΪ��λ

	while (visit)  //�������·��
	{
		cout << "(" << visit->data.x+1 << "," << visit->data.y+1 << ")\n";
		visit = visit->parent();
	}

	system("pause");
	return 0;
}