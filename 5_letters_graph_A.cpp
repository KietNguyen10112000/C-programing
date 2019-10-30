#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include <string.h>
#include<conio.h>

struct memory
{
	int* list;
	int last;
};
typedef struct memory memory;

struct Dinh
{
	int stt;
	char inf[7];
	int bac;
	memory dinh_ke;
	int visited;
	int thanh_phan_lien_thong;
	int distance;
	int father;
};
typedef struct Dinh dinh;
int count = 0;
int dau_hang_doi=0;
int cuoi_hang_doi = 0;
int kich_thuoc_queue = 5757;

//Kiem tra xem 2 dinh co phai dinh ke hay ko
int is_dinh_ke(char* x, char* y)
{
	if (x[0] == y[0] && x[1] == y[1] && x[2] == y[2] && x[3] == y[3] && x[4] != y[4]) return 1;
	if (x[0] == y[0] && x[1] == y[1] && x[2] == y[2] && x[3] != y[3] && x[4] == y[4]) return 1;
	if (x[0] == y[0] && x[1] == y[1] && x[2] != y[2] && x[3] == y[3] && x[4] == y[4]) return 1;
	if (x[0] == y[0] && x[1] != y[1] && x[2] == y[2] && x[3] == y[3] && x[4] == y[4]) return 1;
	if (x[0] != y[0] && x[1] == y[1] && x[2] == y[2] && x[3] == y[3] && x[4] == y[4]) return 1;
	return 0;
}

//Nap du lieu cho bac cua cac dinh
void scan_bac(dinh* ds_dinh)
{
	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < count; j++)
		{
			if (is_dinh_ke(ds_dinh[i].inf, ds_dinh[j].inf) == 1) ds_dinh[i].bac++;
		}
	}
}
//Nap du lieu cho ds dinh ke
void scan_dinh_ke(dinh* ds_dinh)
{
	for (int i = 0; i < count; i++)
	{
		ds_dinh[i].dinh_ke.list = (int*)malloc(ds_dinh[i].bac * sizeof(int));
	}
	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < count; j++)
		{
			if (is_dinh_ke(ds_dinh[i].inf, ds_dinh[j].inf) == 1)
			{
				ds_dinh[i].dinh_ke.list[ds_dinh[i].dinh_ke.last] = ds_dinh[j].stt;
				ds_dinh[i].dinh_ke.last++;
			}
		}
	}
}

int cc = 0;

//Thuat toan explore
void explore(dinh* ds_dinh,dinh x)
{
	ds_dinh[x.stt].visited = 1;
	ds_dinh[x.stt].thanh_phan_lien_thong = cc;
	for (int i = 0; i < x.dinh_ke.last; i++)
	{
		if (ds_dinh[x.dinh_ke.list[i]].visited == 0) explore(ds_dinh, ds_dinh[x.dinh_ke.list[i]]);
	}
}

//DFS
void DFS(dinh* ds_dinh)
{
	for (int i = 0; i < count; i++)
	{
		if (ds_dinh[i].visited == 0)
		{
			cc++;
			explore(ds_dinh, ds_dinh[i]);
		}
	}
}

//Hang doi
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
		for (int i = 0; i < ds_dinh[temp].dinh_ke.last; i++)
		{
			if (ds_dinh[ds_dinh[temp].dinh_ke.list[i]].distance == -1)
			{
				inject(Q, ds_dinh[ds_dinh[temp].dinh_ke.list[i]].stt);
				ds_dinh[ds_dinh[temp].dinh_ke.list[i]].distance = ds_dinh[temp].distance + 1;
				ds_dinh[ds_dinh[temp].dinh_ke.list[i]].father = temp;
			}
		}
	}
}

int main()
{
	//doc flie
	FILE* fptr2;
	errno_t err2;
	err2 = fopen_s(&fptr2, "D:/programing_learing/Toan_hoc_roi_rac/coding/Toan_hoc_roi_rac/5_letter_words.txt", "r");
	if (err2 == 0)
	{
		printf("The file was opened\n");
	}
	else
	{
		printf("The file was not opened\n");
		return -1;
	}
	dinh* ds_dinh;
	ds_dinh = (dinh*)malloc(5757 * sizeof(dinh));
	
	while (!feof(fptr2))
	{
		fgets(ds_dinh[count].inf, 10, fptr2);
		ds_dinh[count].stt = count;
		ds_dinh[count].visited = 0;
		ds_dinh[count].bac = 0;
		ds_dinh[count].dinh_ke.last = 0;
		ds_dinh[count].distance = -1;
		ds_dinh[count].father = -1;
		count++;
	}

	//tao danh sach lien ket
	scan_bac(ds_dinh);
	scan_dinh_ke(ds_dinh);
	printf("\n");
	//DFS
	DFS(ds_dinh);
	printf_s("So thanh phan lien thong:  %d\n", cc);
	printf("\n");
	//Nhap dinh vao ra
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
	int dem = 0;
	while (strcmp(ds_dinh[dem].inf, str1) != 0)
	{
		dem++;
	}
	int dem1 = 0;
	while (strcmp(ds_dinh[dem1].inf, str2) != 0)
	{
		dem1++;
	}

	//BFS
	BFS(ds_dinh, dem1);

	//in duong di
	int dem2 = dem;
	if (ds_dinh[dem].distance == -1)
	{
		printf("\n");
		printf("Khong co duong di giua 2 tu\n");
		printf("\n");
	}
	else
	{
		printf("\n");
		printf("The shortest way is:\n");
		printf("\n");
		for (int i = 0; i < ds_dinh[dem].distance + 1; i++)
		{
			printf("%s", ds_dinh[dem2].inf);
			dem2 = ds_dinh[dem2].father;
			printf("\n");
		}
	}
	system("pause");
	return 0;
}