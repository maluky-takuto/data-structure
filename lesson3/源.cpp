//�Թ�����
#include<iostream>
#include<iomanip>

using namespace std;

#define boundary 1 //��ͼ�߽���

class node;//�ȱ�־һ��

int dirx[5] = { 0,1,0,-1,0 };

int diry[5] = { 0,0,1,0,-1 };

//��������
string s[4] = { "\u2192","\u2193","\u2190","\u2191" };

class node {
public:
	//x, y����
	int x;
	int y;
	int state;//��ʾ״̬��0��ʾ·����-1��ʾ�߽���ϰ���-2��ʾ����ͬ��1��ʾ��ڣ�2��ʾ����
	int dir = 1;//��ʾ����
	int visited = 0;//��ʾ�Ƿ���ʣ�����
	bool an = false;

	node() {
		x = 0;
		y = 0;
		state = 0;
		dir = 1;//����Ĭ�ϴӶ���ʼ
		visited = 0;//Ĭ��û�з���
		an = false;
	}

	void set(int i, int j, int s) {
		x = i;
		y = j;
		state = s;
	}

	void change_state(int s) {
		state = s;
	}

	bool shot();

	void next(int& x, int& y);//����λ��д������
};

//��ͼ
node* map[1020][1020] = {};
//0��ʾ·����1��ʾ��ڣ�2��ʾ���ڣ�-1��ʾ�߽�

int l;
int w;//��ʾ��ͼ�ĳ��Ϳ�
int sum = 0;//��ʾ����

bool node::shot() {//ֻ���ȿ�����ǰλ���ܲ���ͨ��
	if (map[this->y][this->x]->state != -1 && map[this->y][this->x]->visited == 0)
		return true;
	return false;
}

void node::next(int& x, int& y) {
	switch (dir) {
	case 1:
		x++;
		break;
	case 2:
		y++;
		break;
	case 3:
		x--;
		break;
	case 4:
		y--;
		break;
	default:
		return;
		break;
	}
}

//ʹ��һ��ջ�����棬�߹��Ľڵ�����
class stack {//��ϰ˳��ջ
public:
	node* top;
	node* base;
	int size;

	//��ʼ��ջ�ռ�
	stack() {
		base = (node*)malloc(1000 * sizeof(node));//����1000�Ĵ�С
		if (!base)return;
		top = base;
		size = 1000;
	}

	void destroy() {
		if (!base)return;
		delete base;
		return;
	}

	//���ջ
	void clear() {
		while (!this->isempty()) {
			this->pop();
		}
	}

	//�жϿ�ջ��������
	bool isempty() {
		return base == top;
	}

	void push(node e) {
		//���������ж���
		*top++ = e;
		//��Ҫ���ı�һ��״̬
		return;
	}

	//�����أ�����
	void pop() {
		if (isempty())return;
		--top;
		return;
	}

	void pop(node& e) {//��e�����Ǹ�������ֵ
		if (base == top)return;
		e = *--top;
		return;
	}

	void printstack() {
		node* e = this->base;
		while (e != this->top) {
			cout << "(" << e->x << "," << e->y << "," << s[e->dir - 1] << ")" << endl;
			e++;
		}
	}
	//�������캯��
	void operator=(const stack &s) {
		node* e = s.base;
		while (e != s.top) {
			this->push(*e);
			e++;
		}
	}
};

stack arry[100];//����ջ��������������

stack sq;//��ʱջ

//��ʼ����ͼ
static void setmap(int length, int width) {
	//��ʼ���߽�
	//��������ǰ���ԭ��������
	for (int i = 0; i < length + boundary * 2; i++) {
		map[0][i] = new node();
		map[0][i]->set(i, 0, -1);
		map[width + 1][i] = new node();
		map[width + 1][i]->set(i, width + 1, -1);
	}
	for (int i = 0; i < width + boundary * 2; i++) {
		map[i][0] = new node();
		map[i][0]->set(0, i, -1);
		map[i][length + 1] = new node();
		map[i][length + 1]->set(length + 1, i, -1);
	}

	//��ʼ��·��
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < length; j++) {
			map[i + 1][j + 1] = new node();
			map[i + 1][j + 1]->set(j + 1, i + 1, 0);
		}
	}

	map[1][1]->set(1, 1, 1);//�������
	map[width][length]->set(length, width, 2);//����
}

//��ӡ��ͼ
static void printmap(int length, int width) {
	//��ӡ����
	for (int i = 0; i < width + 2; i++) {
		for (int j = 0; j < length + 2; j++) {
			switch (map[i][j]->state) {
			case -1:
				cout << setw(2) << "#";
				break;
			case 0:
				if (map[i][j]->an) {
					cout << setw(2) << s[map[i][j]->dir - 1];//·����
				}
				else {
					cout << setw(2) << " ";//·��
				}
				break;
			case 1:
				cout << setw(2) << "*";
				break;
			case 2:
				cout << setw(2) << "@";
				break;
			default:
				break;
			}
		}
		cout << endl;
	}
}

//��ӡ�⣡
static void printanswer(int length, int width,stack& sq) {
	node* temp ;
	bool flag=false;//��һ�±��
	for (int i = 0; i < width + 2; i++) {
		for (int j = 0; j < length + 2; j++) {
			switch (map[i][j]->state) {
			case -1:
				cout << setw(2) << "#";
				break;
			case 0:
				temp = sq.base;//��ʼ���������ͺ�forѭ��һ��
				//����ջ��
				while (temp != sq.top) {
					if (temp->x == j && temp->y == i) {
						cout << setw(2) <<"\033[1;31m" << s[temp->dir - 1]<<"\033[0m";
						flag = true;
						temp = sq.base;//��ͷ��ʼ����
						break;
					}
					temp++;
				}
				if (!flag)cout << setw(2) << " ";
				flag = false;
				break;
			case 1:
				cout << setw(2) << "*";
				break;
			case 2:
				cout << setw(2) << "@";
				break;
			default:
				break;
			}
		}
		cout << endl;
	}
}

static void walk(int x, int y) {
	//��������
	if (x == l && y == w) {
		for (int i = 0; i < 100; i++) {
			if (arry[i].isempty()) {
				arry[i] = sq;
				break;
			}
		}//���浽ջ����
		sum++;
		return;
	}

	//�ж��ĸ�����
	for (int i = 1; i <= 4; i++) {
		if (map[y + diry[i]][x + dirx[i]]->visited == 0 && map[y + diry[i]][x + dirx[i]]->state != -1) {//�ĳ�Ҫȥ��λ��
			map[y][x]->visited = 1;

			//��ջ
			map[y][x]->dir = i;//��Ҫֱ�Ӹı�dir��ֵ
			sq.push(*map[y][x]);//����ǵ�ǰλ��

			walk(x + dirx[i], y + diry[i]);//����ֱ�Ӹ�x,y��ֵ

 			map[y][x]->visited = 0;//��ԭλ�ã�����Ҫ�����Գ�ջ
			sq.pop();//������ֱ�ӵ���
		}
	}
}

////���Ĺ��ܺ���
//static void fun(node* start, node* end) {
//	sq->init();//��ʼ��һ����ջsq
//	node* curpos = start;//��ǰλ��
//	node* e = new node();//����������ص���ֵ
//	do {
//		//�����ǰλ�ÿ���ͨ��
//		if (curpos->shot()) {//��ǰλ�ÿ�����
//			curpos->visited = 1;//�����㼣
//			//��ջ+��һ��״̬
//			curpos->an = true;
//			sq->push(*curpos);
//			//����ҵ�
//			if (curpos->state == 2) {//˵���ҵ�
//				return;
//			}
//			curpos = curpos->next();
//		}
//		else {//��ǰλ�ò���ͨ��
//			if (!sq->isempty()) {//ջ���գ���ջ
//				e->an = false;
//				sq->pop(*e);
//
//				while (e->dir >= 4 && !sq->isempty()) {
//					e->state = -2;
//
//					e->an = false;
//					sq->pop(*e);//�ٿ���һ���в���
//				}//һֱ���е��Ǹ�
//				if (e->dir < 4) {
//					e->dir++;
//					map[e->y][e->x]->dir++;//����ͬ��
//
//					e->an = true;
//					sq->push(*e);
//
//					curpos = e->next();
//				}
//			}
//		}
//	} while (!sq->isempty());//ջ����
//}

int main() {
	cout << "�����ͼ����" << endl;
	cin >> l >> w;
	setmap(l, w);//��ʼ����ͼ
	printmap(l, w);
	//�����ϰ�������
	cout << "�����ϰ����꣬��q����" << endl;
	int x;
	int y;
	while (true) {
		cin >> x >> y;
		//���������˳�
		if (cin.fail()) {
			cin.clear();
			cin.ignore();
			break;
		}
		map[y][x]->change_state(-1);
		printmap(l, w);
	}
	/*fun(map[1][1], map[y][x]);*/
	walk(1, 1);//����㿪ʼwalk

	for (int i = 0; i < 100; i++) {
		if (!arry[i].isempty()) {//����
			cout << "��" << i + 1 << "�֣�" << endl;
			arry[i].printstack();
			cout << endl;
			printanswer(l, w, arry[i]);
			cout << endl;
		}
	}
	cout << "�ܽ⣺"<<sum << endl;
	return 0;
}