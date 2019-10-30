#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include <string.h>
#include<conio.h>

struct memory
{
	int bac;
	int* list;
	int last;
};
typedef struct memory memory;

struct Dinh
{
	int stt;
	char inf[7];
	memory dinh_ke_xuoi;
	memory dinh_ke_nguoc;
	int visited;
	int ccnum;
	int distance;
	int father;
	int post;
	int pre;
};
typedef struct Dinh dinh;
int count = 0;
//Kiem tra x--->y?(y co la dinh ke cua x hay ko)
int is_dinh_ke(char* x, char* y)
{
	char str1[7], str2[7];
	strcpy_s(str1, sizeof(str1), x);
	strcpy_s(str2, sizeof(str2), y);
	int count = 0;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 1; j < 5; j++)
		{
			if (str2[i] == str1[j])
			{
				str1[j] = ' ';
				count++;
				break;
			}
		}
	}
	if (count == 4) return 1;
	return 0;
}
//scan bac
void scan_bac(dinh* ds_dinh)
{
	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < count; j++)
		{
			if (is_dinh_ke(ds_dinh[i].inf, ds_dinh[j].inf) == 1)
			{
				ds_dinh[i].dinh_ke_xuoi.bac++;
				ds_dinh[j].dinh_ke_nguoc.bac++;
			}
		}
	}
}
//Nap du lieu cho ds dinh ke
void scan_dinh_ke(dinh* ds_dinh)
{
	for (int i = 0; i < count; i++)
	{
		ds_dinh[i].dinh_ke_xuoi.list = (int*)malloc(ds_dinh[i].dinh_ke_xuoi.bac * sizeof(int));
		ds_dinh[i].dinh_ke_nguoc.list = (int*)malloc(ds_dinh[i].dinh_ke_nguoc.bac * sizeof(int));
	}
	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < count; j++)
		{
			if (is_dinh_ke(ds_dinh[i].inf, ds_dinh[j].inf) == 1)
			{
				ds_dinh[i].dinh_ke_xuoi.list[ds_dinh[i].dinh_ke_xuoi.last] = ds_dinh[j].stt;
				ds_dinh[i].dinh_ke_xuoi.last++;
				ds_dinh[j].dinh_ke_nguoc.list[ds_dinh[j].dinh_ke_nguoc.last] = ds_dinh[i].stt;
				ds_dinh[j].dinh_ke_nguoc.last++;
			}
		}
	}
}

int cc = 0;
int clock = 1;
//Thuat toan explore xuoi
void explore_xuoi(dinh* ds_dinh,dinh x)
{
	ds_dinh[x.stt].visited = 1;
	ds_dinh[x.stt].ccnum = cc;
	
	for (int i = 0; i < x.dinh_ke_xuoi.last; i++)
	{
		if (ds_dinh[x.dinh_ke_xuoi.list[i]].visited == 0) explore_xuoi(ds_dinh, ds_dinh[x.dinh_ke_xuoi.list[i]]);
	}
	
}
int find_max_post(dinh* ds_dinh)
{
	int max = ds_dinh[0].post;
	int dem = 0;
	for (int i = 0; i < count; i++)
	{
		if (max < ds_dinh[i].post)
		{
			max = ds_dinh[i].post;
			dem = ds_dinh[i].stt;
			ds_dinh[i].post = -1;
		}
	}
	return dem;
}
//DFS xuoi
void DFS_xuoi(dinh* ds_dinh)
{
	for (int i = 0; i < count; i++)
	{
		ds_dinh[i].visited = 0;
	}
	for (int i = 0; i < count; i++)
	{
		if (ds_dinh[i].visited == 0)
		{
			cc++;
			explore_xuoi(ds_dinh, ds_dinh[find_max_post(ds_dinh)]);
		}
	}
}

//Thuat toan explore nguoc
void explore_nguoc(dinh* ds_dinh, dinh x)
{
	ds_dinh[x.stt].visited = 1;
	//ds_dinh[x.stt].ccnum = cc;
	ds_dinh[x.stt].pre = clock;
	clock++;
	for (int i = 0; i < x.dinh_ke_nguoc.last; i++)
	{
		if (ds_dinh[x.dinh_ke_nguoc.list[i]].visited == 0) explore_nguoc(ds_dinh, ds_dinh[x.dinh_ke_nguoc.list[i]]);
	}
	ds_dinh[x.stt].post = clock;
	clock++;
}

//DFS nguoc
void DFS_nguoc(dinh* ds_dinh)
{
	for (int i = 0; i < count; i++)
	{
		ds_dinh[i].visited = 0;
	}
	for (int i = 0; i < count; i++)
	{
		if (ds_dinh[i].visited == 0)
		{
			//cc++;
			explore_nguoc(ds_dinh, ds_dinh[i]);
		}
	}
}

//Hang doi
int dau_hang_doi=0;
int cuoi_hang_doi = 0;
int kich_thuoc_queue = 5757;
//1 la rong 0 la khong rong
int isEmpty() 
{
	if (cuoi_hang_doi == dau_hang_doi) return 1;
	return 0;
}

//Them vao hang doi phan tu x
void inject(int* hang_doi, int x)
{
	hang_doi[cuoi_hang_doi] = x;
	cuoi_hang_doi = (cuoi_hang_doi + 1) % kich_thuoc_queue;
}
//Bo 1 phan tu khoi hang doi
int eject(int* hang_doi)
{
	int o = hang_doi[dau_hang_doi];
	dau_hang_doi = (dau_hang_doi + 1) % kich_thuoc_queue;
	return o;
}
//BFS
void BFS(dinh* ds_dinh, int x)
{
	int Q[5757];
	ds_dinh[x].distance = 0;
	inject(Q, ds_dinh[x].stt);
	int temp;
	while (isEmpty() == 0)
	{
		temp = eject(Q);
		for (int i = 0; i < ds_dinh[temp].dinh_ke_xuoi.last; i++)
		{
			if (ds_dinh[ds_dinh[temp].dinh_ke_xuoi.list[i]].distance == -1)
			{
				inject(Q, ds_dinh[ds_dinh[temp].dinh_ke_xuoi.list[i]].stt);
				ds_dinh[ds_dinh[temp].dinh_ke_xuoi.list[i]].distance = ds_dinh[temp].distance + 1;
				ds_dinh[ds_dinh[temp].dinh_ke_xuoi.list[i]].father = temp;
			}
		}
	}
}
int main()
{
	//doc flie
	FILE* fptr3;
	errno_t err3;
	err3 = fopen_s(&fptr3, "LINK to your .TXT FILE in your PC", "r");
	if (err3 == 0)
	{
		printf("The file was opened .... DATA is loading...\n\n");
	}
	else
	{
		printf("The file was not opened\n");
		return -1;
	}

	dinh* ds_dinh;
	ds_dinh = (dinh*)malloc(5757 * sizeof(dinh));
	
	while (!feof(fptr3))
	{
		fgets(ds_dinh[count].inf, 10, fptr3);
		ds_dinh[count].stt = count;
		ds_dinh[count].visited = 0;
		ds_dinh[count].dinh_ke_xuoi.bac = 0;
		ds_dinh[count].dinh_ke_xuoi.last = 0;
		ds_dinh[count].dinh_ke_nguoc.bac = 0;
		ds_dinh[count].dinh_ke_nguoc.last = 0;
		ds_dinh[count].distance = -1;
		ds_dinh[count].father = -1;
		count++;
	}
	//DFS
	scan_bac(ds_dinh);
	scan_dinh_ke(ds_dinh);
	DFS_nguoc(ds_dinh);
	DFS_xuoi(ds_dinh);
	
	printf("So thanh phan lien thong manh: %d\n", cc);

	//Dua ra tat ca cac tu thuoc cung thanh phan lien thong
	char str[7];
	printf("\nNhap 1 tu: ");
	gets_s(str, sizeof(str));
	str[5] = '\n';
	str[6] = '\0';
	int dem = 0;
	while (strcmp(ds_dinh[dem].inf, str) != 0)
	{
		dem++;
	}
	printf("\nCac tu thuoc cung thanh phan lien thong la : \n");
	for (int i = 0; i < count; i++)
	{
		if (ds_dinh[i].ccnum == ds_dinh[dem].ccnum) printf("%s", ds_dinh[i].inf);
	}

	//Tim duong di ngan nhat
	char str1[7], str2[7];
	printf("Nhap dinh xuat phat: ");
	gets_s(str1, 6);
	str1[5] = '\n';
	str1[6] = '\0';
	printf("\n");
	printf("Nhap dinh ket thuc: ");
	gets_s(str2, 6);
	str2[5] = '\n';
	str2[6] = '\0';
	int dem1 = 0;
	while (strcmp(ds_dinh[dem1].inf, str1) != 0)
	{
		dem1++;
	}
	int dem2 = 0;
	while (strcmp(ds_dinh[dem2].inf, str2) != 0)
	{
		dem2++;
	}
	//BFS
	BFS(ds_dinh, dem1);
	printf("\n");
	if (ds_dinh[dem2].distance == -1) printf("Khong co duong di\n\n");
	else
	{
		printf("The shortest way is:\n");
		printf("\n");

		/*int* x;*/
		int last = 0;
		/*x = (int*)malloc((ds_dinh[dem2].distance + 1) * sizeof(int));*/
		int x[5757];
		int dem3 = dem2;
		for (int i = 0; i < ds_dinh[dem2].distance + 1; i++)
		{
			x[last] = dem3;
			dem3 = ds_dinh[dem3].father;
			last++;
		}

		last--;
		while (last != 0)
		{
			ds_dinh[x[last]].inf[5] = '\0';
			printf("%s ----> ", ds_dinh[x[last]].inf);
			last--;
		}
		printf("%s", ds_dinh[dem2].inf);
		printf("\n");
	}
	system("pause");
	return 0;
}
