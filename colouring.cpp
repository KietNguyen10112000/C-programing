#include<stdio.h>
#include<stdlib.h>
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
	int mau;
	int visited;
};
typedef struct Dinh dinh;

int top;
int so_canh;

//tan so cua dinh i bang tan_so[i], tan_so chinh la bac cua dinh do
int* quet_tan_so(canh* x)
{
	int* temp;
	temp = (int*)malloc((top+1)* sizeof(int));
	for (int i = 0; i < (top+1); i++)
	{
		temp[i] = 0;
	}
	for (int i = 0; i < so_canh; i++)
	{
			temp[x[i].trai]++;
			temp[x[i].phai]++;
	}
	return temp;
}
//1 neu chinh quy, 0 neu khong chinh quy
int check_graph(dinh* ds)
{
	for (int i = 0; i < top-1; i++)
	{
		if (ds[i].bac != ds[i+1].bac) return 0;
	}
	return 1;
}
int find_min(int* x, int n)
{
	int min = x[0];
	for (int i = 0; i < n; i++)
	{
		if (min > x[i] && x[i] != 0) min = x[i];
	}
	return min;
}
//mau nhho nhat thoa man cua dinh x
int check_mau(int mau,dinh x, dinh* ds_dinh, canh* ds_canh)
{
	for (int i = 0; i < so_canh; i++)
	{
		if (x.inf == ds_canh[i].trai)
		{
			if (mau== ds_dinh[ds_canh[i].phai - 1].mau)
			{
				return 1;
			}
		}
		if (x.inf == ds_canh[i].phai)
		{
			if (mau == ds_dinh[ds_canh[i].trai - 1].mau)
			{
				return 1;
			}
		}
	}
	return 0;
}
int mau_nho_nhat_thoa_man(dinh x, dinh* ds_dinh, canh* ds_canh)
{
	/*int mau[20] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20 };
	for (int i = 0; i < so_canh; i++)
	{
		if (x.inf == ds_canh[i].trai)
		{
			mau[ds_dinh[ds_canh[i].phai - 1].mau - 1] = 21;
		}
		if (x.inf == ds_canh[i].phai)
		{
			mau[ds_dinh[ds_canh[i].trai - 1].mau - 1] = 21;
		}
	}
	int mau_thoa_man = find_min(mau, 20);*/
	int mau_thoa_man = 1;
	for (int i = 0; i < so_canh; i++)
	{
		if (check_mau(mau_thoa_man, x, ds_dinh, ds_canh) == 1) mau_thoa_man++;
	}
	return mau_thoa_man;
}
void to_mau_do_thi_chinh_quy(canh* ds_canh, dinh* ds_dinh)
{
	for (int i = 0; i < top; i++)
	{
		ds_dinh[i].mau = mau_nho_nhat_thoa_man(ds_dinh[i], ds_dinh, ds_canh);
	}
}
struct memory
{
	dinh* ds;
	int last;
};
void refresh_ds(dinh* ds_dinh, canh* ds_canh, struct memory *bo_nho)
{
	for (int j = 0; bo_nho->last != top; j++)
	{
		for (int i = 0; i < so_canh; i++)
		{
			if (bo_nho->ds[j].inf == ds_canh[i].phai && ds_dinh[ds_canh[i].trai - 1].visited == 0)
			{
				ds_dinh[ds_canh[i].trai - 1].visited = 1;
				bo_nho->ds[bo_nho->last] = ds_dinh[ds_canh[i].trai - 1];
				bo_nho->last++;
			}
			if (bo_nho->ds[j].inf == ds_canh[i].trai && ds_dinh[ds_canh[i].phai - 1].visited == 0)
			{
				ds_dinh[ds_canh[i].phai - 1].visited = 1;
				bo_nho->ds[bo_nho->last] = ds_dinh[ds_canh[i].phai - 1];
				bo_nho->last++;
			}
		}
	}
}
int find_bac_max(dinh* ds_dinh)
{
	int max = ds_dinh[0].bac;
	for (int i = 0; i < top; i++)
	{
		if (max < ds_dinh[i].bac) max = ds_dinh[i].bac;
	}
	return max;
}

void to_mau_do_thi_khong_chinh_quy(dinh* ds_dinh, canh* ds_canh)
{
	struct memory* bo_nho;
	bo_nho = (struct memory*)malloc(sizeof(struct memory));
	bo_nho->ds= (dinh*)malloc(top * sizeof(dinh));
	bo_nho->last = 0;
	int bac_max = find_bac_max(ds_dinh);
	int m = 0;
	while (ds_dinh[m].bac == bac_max)
	{
		m++;
	}
	ds_dinh[m].visited = 1;
	bo_nho->ds[bo_nho->last] = ds_dinh[m];
	bo_nho->last++;
	
	refresh_ds(ds_dinh, ds_canh, bo_nho);
	
	for (int i = 0; i < bo_nho->last; i++)
	{
		printf_s("%d\t", bo_nho->ds[i].inf);
	}
	
	for (int i = 0; i < top; i++)
	{
		ds_dinh[bo_nho->ds[bo_nho->last-1].inf-1].mau = mau_nho_nhat_thoa_man(ds_dinh[bo_nho->ds[bo_nho->last-1].inf - 1], ds_dinh, ds_canh);
		bo_nho->last--;
	}
}

int main3()
{
	//doc flie
	FILE* fptr1;
	errno_t err1;
	err1 = fopen_s(&fptr1, "LINK to your .TXT FILE in your PC", "r");
	if (err1 == 0)
	{
		printf("The file was opened\n");
	}
	else
	{
		printf("The file was not opened\n");
		return -1;
	}
	canh* ds_canh;
	fscanf_s(fptr1, "%d", &top);
	fscanf_s(fptr1, "%d", &so_canh);
	ds_canh = (canh*)malloc(so_canh * sizeof(canh));

	for (int i = 0; i < so_canh; i++)
	{
		fscanf_s(fptr1, "%d", &ds_canh[i].trai);
		fscanf_s(fptr1, "%d", &ds_canh[i].phai);
	}
	fclose(fptr1);
	for (int i = 0; i < so_canh; i++)
	{
		printf_s("%d--%d\n", ds_canh[i].trai, ds_canh[i].phai);
	}
	int* tan_so;
	//tan_so= (int*)malloc(top * sizeof(int));
	tan_so = quet_tan_so(ds_canh);
	dinh* ds_dinh;
	ds_dinh = (dinh*)malloc(top * sizeof(dinh));

	for (int i = 0; i < top ; i++)
	{
		ds_dinh[i].bac = tan_so[i+1];
		ds_dinh[i].inf = i + 1;
		ds_dinh[i].mau = 0;
		ds_dinh[i].visited = 0;
	}
	free(tan_so);
	for (int i = 0; i < top; i++)
	{
		printf_s("Dinh %d co bac %d\n", ds_dinh[i].inf, ds_dinh[i].bac);
	}
	if (check_graph(ds_dinh) == 1)
	{
		to_mau_do_thi_chinh_quy(ds_canh, ds_dinh);
	}
	else to_mau_do_thi_khong_chinh_quy(ds_dinh, ds_canh);
	
	printf_s("\n");
	for (int i = 0; i < top; i++)
	{
		printf_s("%d\t", ds_dinh[i].mau);
	}
	printf_s("\n");

	FILE* fptr2;
	errno_t err2;
	err2 = fopen_s(&fptr2, "LINK to your out put .DOT FILE in your PC", "w");
	if (err2 == 0)
	{
		printf("The file was opened\n");
	}
	else
	{
		printf("The file was not opened\n");
		return -1;
	}

	char mau[][20] = { "red","green","blue","SlateBlue2","yellow","purple","gray","lawn green","YellowGreen","light salmon","hot pink",
		"orange","snow1","azure1","SlateGray3","gold4","light green","tan4","DodgerBlue1","MistyRose1" };

	fprintf(fptr2, "%s","graph dothi \n{\n" );
	for (int i = 0; i < top; i++)
	{
		fprintf(fptr2, "%d [fillcolor=%s, style=filled];\n", ds_dinh[i].inf, mau[ds_dinh[i].mau - 1]);
	}
	for (int i = 0; i < so_canh; i++)
	{
		fprintf(fptr2, "%d--%d;\n", ds_canh[i].trai, ds_canh[i].phai);
	}
	fprintf(fptr2, "%s", "}\n");

	system("pause");
	return 0;
}
