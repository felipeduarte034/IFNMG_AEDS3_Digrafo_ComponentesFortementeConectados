#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#define BRANCO 0
#define CINZA 1
#define PRETO 2
#define RED 2
#define BLUE 1

/**
 * BRANCO - nodo ainda não encontrado
 * CINZA  - nodo encontrado
 * PRETO  - todos os vizinhos verificados
 */

//N --> vertices de 0 até N-1 [0,N)
//M --> m linhas

using namespace std;

class Grafo
{
public:
	int n; //quantidade de nodos no grafo
	vector<vector<int>> adj; //lista de adjacentes
	vector<vector<int>> adj_inv; //lista de adjacentes invertida

	//nodo [0] --> [1] [4]
	//nodo [1] --> [0] [5]

	Grafo(int quant_vertices)
	{
		n = quant_vertices;
		for (int i = 0; i < quant_vertices; i++)
		{
			adj.push_back(vector<int>());
			adj_inv.push_back(vector<int>());
		}
	}
	void criarAresta(int x, int y)
	{
		adj[x].push_back(y);
		adj_inv[y].push_back(x);
	}
	void printAdj()
	{
		int counter=0;
		for(vector<int> v : adj)
		{
			cout << counter++ << ": ";
			for(int x : v)
			{
				cout << x << ", ";
			}
			cout << endl;
		}
	}
};
class Info
{
public:
	int x;
	int y;
	Info(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
};
bool removeByValue(int value, vector<int> &v)
{
	for(int i=0; i<v.size(); i++)
	{
		if(value == v[i])
		{
			v.erase(v.begin() + i);
			return true;
		}
	}
	return false;
}

//------------------------------------------------------------------------------

void DFS_rec(Grafo *g, int v, vector<int> &cor, vector<int> &fim, int*time)
{
	vector<int> ini(g->n,-1);
	*time = (*time)+1;
	ini[v] = *time;
	cor[v] = CINZA;
	for(int i=0; i < g->adj[v].size(); ++i)
	{
		int w = g->adj[v][i];
		if(cor[w]==BRANCO)
		{
			DFS_rec(g,w,cor,fim,time);
		}
	}
	*time = (*time)+1;
	fim[v] = *time;
}

void DFS(Grafo *g, vector<int> &fim, int*time) //percorre todos os VERTICES do grafo
{
	vector<int> cor(g->n, BRANCO);
	for(int v=0; v<g->n; ++v)
	{
		if(cor[v]==BRANCO)
			DFS_rec(g,v,cor,fim,time);
	}
}

void DFS2(Grafo *g, int v, vector<int> &cor)
{
	cor[v]=CINZA;
	for(int i=0; i<g->adj_inv[v].size(); ++i)
	{
		int w = g->adj[v][i];
		if(cor[w]==BRANCO)
		{
			cout << w << ", ";
			DFS2(g,w,cor);
		}
	}
	cor[v]=2;
}

int countCFC(Grafo *g)
{
	int t=0;
	int*time=&t;
	vector<int> fim(g->n,-1);
	DFS(g,fim,time);

	//vector<int> vert_ord = ordena_term(fim, fim.size());
	sort(fim.begin(),fim.end());
	vector<int> cor(g->n, BRANCO);
	//cor.clear();
	//cor.resize(g->n,0);

	*time=0;
	int ncomp = 0;
	for(int i=0; i<g->n; ++i)//percorre todos os VERTICES do grafo
	{
		//int v = vert_ord[i];
		int v = fim[i];
		if(cor[v]==BRANCO)
		{
			cout << "componentes: ";
			DFS2(g,v,cor); //grafo invertido
			ncomp++;
		}
	}
	return ncomp;
}

Info *DecodificaInstrucao(string line)
{
	int x = 0;
	int y = 0;

	size_t pos = line.find(" ");
	x = atoi(line.substr(0, pos).c_str());
	y = atoi(line.substr(pos).c_str());

	return new Info(x, y);
}

int main(int argc, char const *argv[])
{
	char line[50];
	int counter = 0;
	Grafo *g;
	Info *info = NULL;
	int m=0;//numero de linhas

	while (scanf("%[^\n]\n", line) == 1)
	{
		//cout << line << endl;
		counter++;
		if (counter == 1)
		{
			info = DecodificaInstrucao(line);
			m = info->y;
			g = new Grafo(info->x);
			continue;
		}
		info = DecodificaInstrucao(line);
		g->criarAresta(info->x, info->y);

		if(counter == m+1)//+1 devido a primeira linha ser parametro
			break;
	}

	//cout << (isConexo(g,0) ? "Conexo" : "Desconexo") << endl;

	int k = countCFC(g); 
	cout << "k: " << k << " " << endl;

	//g->printAdj();

	return 0;
}
