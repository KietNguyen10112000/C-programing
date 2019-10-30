#include<stdlib.h>
#include<stdio.h>
#include<errno.h>

struct Canh
{
	int trai;
	int phai;
};
typedef struct Canh canh;

struct Dinh
{
	int inf;
	int bac;
};

typedef struct Dinh dinh;
int top;

struct code_tree
{
	int* a;
	int last;
};

typedef struct code_tree code_tree;

struct memory
{
	int* a;
	int last;
};
typedef struct memory memory;

//tan so cua dinh i bang tan_so[i], tan_so chinh la bac cua dinh do
int* quet_tan_so(canh* x) 
{
	int* temp;
	temp = (int*)malloc(top * sizeof(int));
	for (int i = 0; i < top; i++)
	{
		temp[i] = 0;
	}
	for (int i = 0; i < top - 1; i++)
	{
		if (x[i].trai >= 0 && x[i].phai >= 0)
		{
			temp[x[i].trai]++;
			temp[x[i].phai]++;
		}
	}
	return temp;
}
//tim min mang x kick thuoc n
int find_min(code_tree* bo_nho) 
{
	int min = bo_nho->a[0];
	for (int i = 0; i <bo_nho->last; i++)
	{
		if (min > bo_nho->a[i]) min = bo_nho->a[i];
	}
	return min;
}
//lam moi danh sach dinh va canh
void refresh_ds_dinh(dinh* ds_dinh, canh* ds_canh,code_tree*bo_nho,memory *prufer_code)
{
	for (int i = 0; i < top - 1; i++)
	{
		if (ds_dinh[i].bac == 1)
		{
			bo_nho->a[bo_nho->last] = ds_dinh[i].inf;
			bo_nho->last++;
		}
	}
	int temp = find_min(bo_nho);
	for (int i = 0; i < top - 1; i++)
	{
		
		if ((ds_canh[i].phai == temp) || (ds_canh[i].trai == temp))
		{
			ds_dinh[ds_canh[i].trai-1].bac--;
			ds_dinh[ds_canh[i].phai-1].bac--;
		}
	}
	int count = 0;
	int kq;
	while (ds_canh[count].phai != temp && ds_canh[count].trai != temp)
	{
		count++;
	}
	if (ds_canh[count].phai == temp) kq = ds_canh[count].trai;
	else kq = ds_canh[count].trai;
	prufer_code->a[prufer_code->last] = kq;
	prufer_code->last++;
	
	for (int i = 0; i <= bo_nho->last; i++)
	{
		bo_nho->a[i] = top + 1;
		bo_nho->last--;
	}
}
//giai ca bai
void find_result(dinh* ds_dinh,canh*ds_canh,code_tree*bo_nho, memory* p1)
{
	while (p1->last!=top-2)
	{
		refresh_ds_dinh(ds_dinh, ds_canh,bo_nho,p1);
	}
}

int main2()
{
	//doc flie
	FILE* fptr;
	errno_t err;
	err = fopen_s(&fptr, "LINK to your .TXT FILE in your PC", "r");
	if (err == 0)
	{
		printf("The file was opened\n");
	}
	else
	{
		printf("The file was not opened\n");
		return -1;
	}
	canh* ds_canh;
	fscanf_s(fptr, "%d", &top);
	ds_canh = (canh*)malloc((top - 1) * sizeof(canh));

	for (int i = 0; i < top - 1; i++)
	{
		fscanf_s(fptr, "%d", &ds_canh[i].trai);
		fscanf_s(fptr, "%d", &ds_canh[i].phai);
	}
	for (int i = 0; i < top - 1; i++)
	{
		printf_s("%d--%d\n", ds_canh[i].trai, ds_canh[i].phai);
	}
	int* tan_so;
	tan_so = quet_tan_so(ds_canh);
	dinh* ds_dinh;
	ds_dinh = (dinh*)malloc((top-1) * sizeof(dinh));
	for (int i = 0; i < top-1; i++)
	{
		ds_dinh[i].bac = tan_so[i+1];
		ds_dinh[i].inf = i+1;
	}
	free(tan_so);
	printf_s("\n");
	for (int i = 0; i < top-1; i++)
	{
		printf_s("Dinh %d co bac %d\n", ds_dinh[i].inf,ds_dinh[i].bac);
	}
	//cap phat dong
	memory *prufer_code;
	prufer_code = (memory*)malloc(sizeof(memory));
	prufer_code->a = (int*)malloc((top) * sizeof(int));
	prufer_code->last = 0;

	code_tree* bo_nho;
	bo_nho = (code_tree*)malloc(sizeof(code_tree));
	bo_nho->a = (int*)malloc((top) * sizeof(int));
	bo_nho->last = 0;

	find_result(ds_dinh, ds_canh,bo_nho, prufer_code);
	
	printf_s("\n");
	printf_s("Prufer code cua cay la:\n");
	printf_s("\n");
	for (int i = 0; i < top - 2; i++)
	{
		printf_s("%d\t", prufer_code->a[i]);
	}
	printf_s("\n");
	printf_s("\n");
	system("pause");
	return 0;
}
