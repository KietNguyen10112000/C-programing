#include <stdio.h>
#include <stdlib.h>

struct vase
{
	int vase1;
	int vase2;
	int vase3;
};

int max_vase1;
int max_vase2;
int max_vase3;

typedef struct vase set_of_water;

struct node
{
	set_of_water inf;
	struct node* next;
};

typedef struct node* Node;

set_of_water create_set_of_water(int x, int y, int z)
{
	set_of_water temp;
	temp.vase1 = x;
	temp.vase2 = y;
	temp.vase3 = z;
	return temp;
}

Node create(set_of_water x)
{
	Node temp;
	temp = (Node)malloc(sizeof(struct node));
	temp->inf.vase1 = x.vase1;
	temp->inf.vase2 = x.vase2;
	temp->inf.vase3 = x.vase3;
	temp->next = NULL;
	return temp;
}

void add_tail(Node first, set_of_water state)
{
	Node temp, p;
	temp = create(state);
	p = first;
	while (p->next != NULL)
	{
		p = p->next;
	}
	p->next = temp;
}


int* pour(int a, int b, int max_b)
//do binh a sang binh b tra lai muc nuoc binh a,b
//lv_water[0]=a;lv_water[1]=b;
{
	int* lv_water;
	lv_water = (int*)malloc(2 * sizeof(int));
	if (b == max_b)
	{
		lv_water[0] = a;
		lv_water[1] = b;
	}
	else
	{
		if ((max_b - b) == a)
		{
			lv_water[0] = 0;
			lv_water[1] = max_b;
		}
		else if ((max_b - b) > a)
		{
			lv_water[0] = 0;
			lv_water[1] = b + a;
		}
		else
		{
			lv_water[0] = a - (max_b - b);
			lv_water[1] = max_b;
		}
	}
	return lv_water;
}

Node produce(Node x)
//tu x sinh ra 1 chuoi cac bo so tm de bai
{
	int b1, b2, b3;
	b1 = x->inf.vase1;
	b2 = x->inf.vase2;
	b3 = x->inf.vase3;
	Node temp;
	temp = create(x->inf);
	set_of_water new1;
	int* lv;

	//do b2,b3 sang b1
	lv = pour(b2, b1, max_vase1);
	new1 = create_set_of_water(lv[1], lv[0], b3);
	add_tail(temp, new1);

	lv = pour(b3, b1, max_vase1);
	new1 = create_set_of_water(lv[1], b2, lv[0]);
	add_tail(temp, new1);

	//do b1,b3 sang b2
	lv = pour(b1, b2, max_vase2);
	new1 = create_set_of_water(lv[0], lv[1], b3);
	add_tail(temp, new1);

	lv = pour(b3, b2, max_vase2);
	new1 = create_set_of_water(b1, lv[1], lv[0]);
	add_tail(temp, new1);

	//do b1,b2 sang b3
	lv = pour(b1, b3, max_vase3);
	new1 = create_set_of_water(lv[0], b2, lv[1]);
	add_tail(temp, new1);

	lv = pour(b2, b3, max_vase3);
	new1 = create_set_of_water(b1, lv[0], lv[1]);
	add_tail(temp, new1);

	return temp;
}

int check(set_of_water x, Node first)
{
	Node p1 = first;
	while (p1 != NULL)
	{
		if (x.vase1 == p1->inf.vase1
			&& x.vase2 == p1->inf.vase2
			&& x.vase3 == p1->inf.vase3) return 1;
		p1 = p1->next;
	}
	return 0;
}

void create_list(Node first, Node x)
{
	Node temp;
	temp = produce(x);
	Node p1, p2;
	p1 = first;
	p2 = temp;
	while (p2 != NULL)
	{
		if (check(p2->inf, first) == 0)
			add_tail(first, p2->inf);
		p2 = p2->next;
	}
}

void find_all_result(Node first)
{
	Node p1;
	p1 = first;
	while (p1 != NULL)
	{
		create_list(first, p1);
		p1 = p1->next;
	}
}

int main1()
{
	set_of_water first;

	printf("Nhap dung tich 3 binh\n");
	printf("Dung tich binh 1 : ");
	scanf_s("%d", &max_vase1);
	printf("Dung tich binh 2 : ");
	scanf_s("%d", &max_vase2);
	printf("Dung tich binh 3 : ");
	scanf_s("%d", &max_vase3);

	printf("Nhap muc nuoc cua 3 binh\n");
	printf("Muc nuoc binh 1 : ");
	scanf_s("%d", &first.vase1);
	printf("Muc nuoc binh 2 : ");
	scanf_s("%d", &first.vase2);
	printf("Muc nuoc binh 3 : ");
	scanf_s("%d", &first.vase3);
	printf("Tat ca truong hop co the xay ra :\n");

	Node ds;
	ds = create(first);

	find_all_result(ds);

	Node p;
	p = ds;
	while (p != NULL)
	{
		printf("(%d;%d;%d)\n", p->inf.vase1, p->inf.vase2, p->inf.vase3);
		p = p->next;
	}

	set_of_water x;
	printf("Nhap muc nuoc can tim \n");
	printf("Muc nuoc binh 1 : ");
	scanf_s("%d", &x.vase1);
	printf("Muc nuoc binh 2 : ");
	scanf_s("%d", &x.vase2);
	printf("Muc nuoc binh 3 : ");
	scanf_s("%d", &x.vase3);

	if (check(x, ds) == 1) printf("Co the dat duoc\n");
	else printf("Khong the dat duoc\n");

	system("pause");
	return 0;
}
