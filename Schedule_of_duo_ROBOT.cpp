#include<stdio.h>
#include<stdlib.h>
#include<errno.h>

int n; //n<100
int so_canh;
int khoang_cach[100][100];

//tra ve trong so u--v
int trong_so_u_v(int u, int v)
{
	return khoang_cach[u][v];
}

struct dinh
{
	int mac;
	int dinh_ke[100];
	int so_dinh_ke;
};
typedef struct dinh ptu_dinh;
int last;

int find_vi_tri_min(int* V, int* ds)//,int size_ds)
{
	int min = ds[V[0]];
	int vt=V[0];
	for (int i = 0; i < last; i++)
	{
		if (min > ds[V[i]])
		{
			min = ds[V[i]];
			vt = V[i];
		}
	}
	//printf("%d\n", vt);
	return vt;
}

//Xoa ptu co gia tri = gt khoi tap V
void xoa_ptu(int* V, int gt)
{
	int count = 0;
	while (V[count] != gt)
	{
		count++;
	}
	for (int i = count; i < last; i++)
	{
		//if(i+1<*last)
		V[i] = V[i + 1];
	}
	last--;
}
#define vo_cung 9999999;
int* distance;
int* pre;
int* k;
void Dijkstra(ptu_dinh* dinh,int s)
{
	last = n;
	int* V;
	V = (int*)malloc(n * sizeof(int));
	distance = (int*)malloc(n * sizeof(int));
	pre = (int*)malloc(n * sizeof(int));
	k = (int*)malloc(n * sizeof(int));
	for (int i = 0; i < n; i++)
	{
		distance[i] = vo_cung;
		pre[i] = -1;
		k[i] = 0;
		V[i] = i;
	}
	distance[s] = 0;
	
	while (last!=0)
	{
		int u = find_vi_tri_min(V,distance);
		//printf("%d\n", u);
		k[u] = 1;
		xoa_ptu(V, u);
		for (int i = 0; i < dinh[u].so_dinh_ke; i++)
		{
			if (k[dinh[u].dinh_ke[i]] == 0)
			{
				if (distance[dinh[u].dinh_ke[i]] > distance[u] + trong_so_u_v(u, dinh[u].dinh_ke[i]))
				{
					distance[dinh[u].dinh_ke[i]] = distance[u] + trong_so_u_v(u, dinh[u].dinh_ke[i]);
					pre[dinh[u].dinh_ke[i]] = u;
				}
			}
		}

	}
}

int distance_u_v(ptu_dinh* dinh, int u, int v)
{
	Dijkstra(dinh, u);
	return distance[v];
}
struct Toa_do_2_robot
{
	int RobotA;
	int RobotB;
	int pre;
};
typedef struct Toa_do_2_robot Toa_do;
struct memory
{
	Toa_do* ds;
	int last;
};
typedef struct memory memory;
memory* Move(ptu_dinh* dinh, int robA, int robB,int min_distance)
{
	memory* Bo_nho;
	Bo_nho = (memory*)malloc(sizeof(memory));
	int dem = dinh[robA].so_dinh_ke + dinh[robB].so_dinh_ke;
	Bo_nho->ds = (Toa_do*)malloc(dem * sizeof(Toa_do));
	int p = 0;
	for (int i = 0; i < dinh[robA].so_dinh_ke; i++)
	{
		if (distance_u_v(dinh, dinh[robA].dinh_ke[i], robB) > min_distance)
		{
			Bo_nho->ds[p].RobotA = dinh[robA].dinh_ke[i];
			Bo_nho->ds[p].RobotB = robB;
			Bo_nho->ds[p].pre = 0;
			p++;
		}
	}
	for (int i = 0; i < dinh[robB].so_dinh_ke; i++)
	{
		if (distance_u_v(dinh, dinh[robB].dinh_ke[i], robA) > min_distance)
		{
			Bo_nho->ds[p].RobotA = robA;
			Bo_nho->ds[p].RobotB = dinh[robB].dinh_ke[i];
			Bo_nho->ds[p].pre = 0;
			p++;
		}
	}
	Bo_nho->last = p;
	return Bo_nho;
}
//Check xem da co hay chua
int check(memory* Du_lieu, Toa_do x)
{
	for (int i = 0; i < Du_lieu->last; i++)
	{
		if (Du_lieu->ds[i].RobotA == x.RobotA && Du_lieu->ds[i].RobotB == x.RobotB) return 1;
	}
	return 0;
}

//lam moi Du_lieu
void refresh(ptu_dinh* dinh, int robA, int robB, memory* Du_lieu,int min_distance,int locate)
{
	memory* A;
	A = Move(dinh, robA, robB, min_distance);
	for (int i = 0; i < A->last; i++)
	{
		if (check(Du_lieu, A->ds[i]) == 0)
		{
			Du_lieu->ds[Du_lieu->last] = A->ds[i];
			Du_lieu->ds[Du_lieu->last].pre = locate;
			Du_lieu->last++;
		}
	}
}

void find_result(ptu_dinh* dinh, memory* Du_lieu, int min_distance)
{
	for(int i = 0; i < Du_lieu->last; i++)
	{
		refresh(dinh, Du_lieu->ds[i].RobotA, Du_lieu->ds[i].RobotB, Du_lieu, min_distance,i);
	}
}
int main()
{
	//doc flie
	FILE* fptr4;
	errno_t err4;
	err4 = fopen_s(&fptr4, "LINK to your .TXT file in your PC", "r");
	if (err4 == 0)
	{
		printf("The file was opened .... DATA is loading...\n\n");
	}
	else
	{
		printf("The file was not opened\n");
		return -1;
	}

	fscanf_s(fptr4, "%d", &n);
	fscanf_s(fptr4, "%d", &so_canh);

	ptu_dinh* dinh;
	dinh = (ptu_dinh*)malloc(n * sizeof(ptu_dinh));

	for (int i = 0; i < n; i++)
	{
		dinh[i].mac = i;
		dinh[i].so_dinh_ke = 0;
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			khoang_cach[i][j] = -1;
		}
	}
	for(int i=0;i<so_canh;i++)
	{
		int x, y,z;
		fscanf_s(fptr4, "%d", &x);
		fscanf_s(fptr4, "%d", &y);
		fscanf_s(fptr4, "%d", &z);
		khoang_cach[x][y] = z;
		khoang_cach[y][x] = z;
		dinh[x].dinh_ke[dinh[x].so_dinh_ke] = y;
		dinh[x].so_dinh_ke++;
		dinh[y].dinh_ke[dinh[y].so_dinh_ke] = x;
		dinh[y].so_dinh_ke++;
	}
	int p, q, x, y, r; 
	fscanf_s(fptr4, "%d", &p);
	fscanf_s(fptr4, "%d", &q);
	fscanf_s(fptr4, "%d", &x);
	fscanf_s(fptr4, "%d", &y);
	fscanf_s(fptr4, "%d", &r);
	
	memory* Du_lieu;
	Du_lieu = (memory*)malloc(sizeof(memory));
	Du_lieu->ds = (Toa_do*)malloc(10000 * sizeof(Toa_do));
	Du_lieu->last = 1;
	
	Du_lieu->ds[0].RobotA = p;
	Du_lieu->ds[0].RobotB = q;
	Du_lieu->ds[0].pre = -1;
	
	find_result(dinh, Du_lieu, r);
	
	Toa_do unknown;
	unknown.RobotA = x;
	unknown.RobotB = y;

	if(check(Du_lieu, unknown)==0) printf("KHONG THE DI DUOC");
	else
	{
		printf("\n");
		int dem = 0;

		while (Du_lieu->ds[dem].RobotA != x || Du_lieu->ds[dem].RobotB != y)
		{
			dem++;
		}

		int* mang;
		mang = (int*)malloc(dem * sizeof(int));
		int dem2 = 0;
		while (dem != -1)
		{
			mang[dem2] = dem;
			//printf("( %d , %d ) Father: %d\n", Du_lieu->ds[dem].RobotA, Du_lieu->ds[dem].RobotB, Du_lieu->ds[dem].pre);
			dem = Du_lieu->ds[dem].pre;
			dem2++;
		}
		printf("Lich di chuyen\t    Khoang cach\n");
		for (int i = dem2 - 1; i >= 0; i--)
		{
			printf("%d\t%d\t\t%d\n", Du_lieu->ds[mang[i]].RobotA, Du_lieu->ds[mang[i]].RobotB,
				distance_u_v(dinh, Du_lieu->ds[mang[i]].RobotA, Du_lieu->ds[mang[i]].RobotB));
		}
	}
	printf("\n");
	system("pause");
	return 0;
}
