#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#define BRANCO 0
#define CINZA 1
#define PRETO 2

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

	vector<int> ini; //lista de tempos de inicio
	vector<int> fim; //lista de tempos de termino
	vector<int> container; //lista nodos

	//nodo [0] --> [1] [4]
	//nodo [1] --> [0] [5]

	Grafo(int quant_vertices)
	{
		n = quant_vertices;
		for (int i = 0; i < quant_vertices; i++)
		{
			adj.push_back(vector<int>());
			adj_inv.push_back(vector<int>());
			ini.push_back(-1);
			fim.push_back(-1);
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
			cout << counter++ << "--> ";
			for(int x : v)
			{
				cout << x << ", ";
			}
			cout << endl;
		}
	}
	void printTimer()
	{
		for(int t=0; t<ini.size(); t++)
		{
			cout << t << ": [" << ini[t] << "/" << fim[t] << "]" << endl;
		}
	}
	void printContainer()
	{
		cout << "container: ";
		for(int c : container)
		{
			cout << c << ",";
		}
		cout<<endl;
	}
	void printEndTime()
	{
		cout << "tempo de fim: ";
		/*for(int c : container)
		{
			cout << "[n:" << c << ",et:"<<fim[c]<<"] - ";
		}*/
		for(int c : container)
		{
			cout <<fim[c]<<",";
		}
		cout<<endl;
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

void DFS(Grafo *g, int v, vector<int> &cor, int *timer)
{
	*timer=(*timer)+1;
	g->ini[v]=(*timer);
	cor[v]=CINZA;
	for(int i=0; i<g->adj[v].size(); ++i)
	{
		int w = g->adj[v][i];
		if(cor[w]==BRANCO)
			DFS(g,w,cor,timer);
	}
	*timer=(*timer)+1;
	g->fim[v]=(*timer);
	g->container.push_back(v); //armazenar em um container, fila ou pilha
}

void DFS(Grafo *g)
{
	vector<int> cor(g->n,0);
	int t=0;
	int *timer=&t;
	for(int v=0; v<g->n; ++v) //percorre todos os vertices do grafo
	{
		if(cor[v]==BRANCO)
			DFS(g,v,cor,timer);
	}
}


void DFS_inv(Grafo *g, int v, vector<int> &cor)
{
	cout << v << " ";
	//*timer=(*timer)+1;
	//g->ini[v]=(*timer);
	cor[v]=CINZA;
	for(int i=0; i<g->adj_inv[v].size(); ++i)
	{
		int w = g->adj_inv[v][i];
		if(cor[w]==BRANCO)
		{
			DFS_inv(g,w,cor);
		}	
	}
	//*timer=(*timer)+1;
	//g->fim[v]=(*timer);
	//g->container.push_back(v); //armazenar em um container, fila ou pilha
}
void DFS_inv(Grafo *g)
{
	vector<int> cor(g->n,0);
	int t=g->container.size()-1;
	
	for(int v=t; v>=0; v--) //percorre todos os vertices do grafo - seguindo o container
	{
		if(cor[g->container[v]]==BRANCO)
		{
			DFS_inv(g,g->container[v],cor);
			cout << endl;
		}
	}
}

//------------------------------------------------------------------------------

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

	//int k = countCFC(g); 
	//cout << "k: " << k << " " << endl;

	DFS(g);

	cout << "Componentes Fortemente Conexas:"<<endl;
	DFS_inv(g);

	/*cout<<"--------------------------"<<endl;
	g->printAdj();
	cout<<"--------------------------"<<endl;
	g->printTimer();
	cout<<"--------------------------"<<endl;
	g->printContainer();
	cout<<"--------------------------"<<endl;
	g->printEndTime();*/

	return 0;
}
