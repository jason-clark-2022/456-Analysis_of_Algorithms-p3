// Jason Clark CS 456 P3

#include <iostream>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <limits>
#include <chrono>
#include <thread>
#include <queue>

using namespace std;

struct Edge;
struct Node;
class Graph
{
    public: 
        vector<Node> graph;
        void addEdge(int u, int v, double weight);
        void changeEdge(int u, int v, double weight);
        double maxFlow(int s, int t);
        vector<int> bfs(int startNode, int endNode);
};

Graph readFile(string inFile);

int main()
{
    string f1 = "Project3Graph1.txt";
    string f2 = "Project3Graph2.txt";
    
    int source, sink;
    double maxFlow;
    string fileName;

    cout << "Enter the name of the file you would like to run:\n>"; 
    cin >> fileName;
    Graph g = readFile(fileName);
    int s = g.graph.size();
    cout << "Enter the source and sink (recommended [0," << g.graph.size()-1 << "]):\n>";
    cin >> source;
    cout << ">";
    cin >> sink;
    
    maxFlow = g.maxFlow(source,sink);
    cout << "The max flow is " << maxFlow << endl;
    return 0;
}

struct Edge
{
    int u;  
    int v;  
    double weight;
};

struct Node
{
    int id;             
    vector<Edge> edges; 
    double key;         
    int parent;         
    bool visited;       
};


vector<Node> graph;

void Graph::addEdge(int u, int v, double weight)
{
    Edge edge;
    Node node;

    edge.u = u;
    edge.v = v;
    edge.weight = weight;
    
    if(graph.size()>=u+1) 
    {
        graph[u].edges.push_back(edge);   
    }
    else 
    {
        node.id = u;
        if(!(u==-1||v==-1))
            node.edges.push_back(edge);
        node.key = 0;  
        node.parent = -1; 
        node.visited = false;
        graph.push_back(node);
    }
}

void Graph::changeEdge(int u, int v, double weight)
{
    int indexOfV = -1;
    for(int i = 0; i < graph[u].edges.size(); i++)
    {
        if(graph[u].edges[i].v == v)
        {
            indexOfV = i;
        }
    }
    
    if(indexOfV == -1)
    {
        cerr << "Edge from " << u << " to " << v << " not found.\n";
        return;
    }
    graph[u].edges[indexOfV].weight = graph[u].edges[indexOfV].weight - weight;
    return;
}

double Graph::maxFlow(int s, int t)
{
    double maxFlow = 0;
    int count = 0;
    while(true)
    {   
        count++;
        Edge currEdge;
        vector<int> path = bfs(s, t);
        if(path[0] == -1 || count>10)
        {
            break;
        }
        reverse(path.begin(), path.end());
        double bottleneck = numeric_limits<double>::max();
        for(int i = 0; i < path.size()-1; i++)
        {
            for(int j = 0; j < graph[path[i]].edges.size(); j++) 
            {
                if( graph[path[i]].edges[j].v == path[i+1]) 
                {
                    currEdge = graph[path[i]].edges[j];
                }
            }
            bottleneck = min(bottleneck, currEdge.weight);
        }

        cout << "The augmenting path is ";
        for(int i = 0; i < path.size(); i++)
        {
            if(i != path.size()-1)
            {
                cout << path[i] << ", ";
            }
            else
            {
                cout << path[i] << " capacity = " << bottleneck << endl;
            }
            
        }
        
        maxFlow += bottleneck;
        for(int i = 0; i < path.size()-1; i++)
        {
            changeEdge(path[i], path[i+1], bottleneck);
        }
    }
    return maxFlow;
}

vector<int> Graph::bfs(int startNode, int endNode)  
        {
            for(int i = 0; i < graph.size(); i++)
            {
                graph[i].visited = false;
            }

            queue<Node> q;
            graph[startNode].visited = true;
            q.push(graph[startNode]);
            graph[startNode].parent = -1;
            while(!q.empty())
            {
                startNode = q.front().id;
                q.pop();
                
                int size = graph[startNode].edges.size();
                for(int i = 0; i < size; i++)
                {
                    int v = graph[startNode].edges[i].v;
                    if(!graph[v].visited && graph[startNode].edges[i].weight > 0)
                    {
                        graph[v].visited = true;
                        q.push(graph[v]);
                        graph[v].key = graph[startNode].key + 1;
                        graph[v].parent = graph[startNode].id;
                    }
                }
            }

            vector<int> path;
            if(!graph[endNode].visited)
            {
                path.push_back(-1);
            }
            for(int i = endNode; i != -1; i = graph[i].parent)
            {
                path.push_back(i);
            }
            return path;

        }


void printGraph(Graph g)
{
    cout << "Current Graph\n";
    for(int i = 0; i < g.graph.size(); i++)
    {
        cout << g.graph[i].id << " ";
        for(int j = 0; j < g.graph[i].edges.size(); j++)
        {

            cout << g.graph[i].edges[j].v << " " << g.graph[i].edges[j].weight << " ";
        }
        cout << endl;
    }
}

Graph readFile(string inFile)
{
    Graph graph;
    size_t pos = 0;
    string delimiter = " ";
    string newLine;
    string token;
    int u, v, weight;
    bool startNewLine;

    ifstream file;
    file.open(inFile);
    if(file.is_open())
    {
        while(getline(file, newLine))
        {
            Edge edge;
            Node node;
            pos = 0;
            startNewLine = true;
            while((pos = newLine.find(delimiter)) != string::npos)
            {
                if(startNewLine == true)
                {
                    token = newLine.substr(0, newLine.find(delimiter));
                    u = stoi(token);
                    newLine.erase(0, pos+delimiter.length());
                    startNewLine = false;
                }
                else
                {
                    token = newLine.substr(0, newLine.find(delimiter));
                    v = stoi(token);
                    newLine.erase(0, pos+delimiter.length());
                    
                    if((pos = newLine.find(delimiter)) != string::npos)
                    {
                        token = newLine.substr(0, newLine.find(delimiter));
                        weight = stod(token);
                        newLine.erase(0, pos+delimiter.length());
                    }
                    else
                    {
                        weight = stod(newLine);
                    }
                    
                    graph.addEdge(u,v,weight);
                }
            }

            if(startNewLine == true)
            {
                u = stoi(newLine);
                graph.addEdge(u,-1,-1); 
            }
        }
    }
    else
    {
        cerr << "File not found\n";
    }
    
    return graph;
}