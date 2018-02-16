//
// Created by Toreno on 11.02.2018.
//

#ifndef MYGRAPHSOLUTION_GRAPH_H
#include <iostream>
#include <vector>
#include <set>
#include <utility>
#include <algorithm>
using namespace std;

// множество классов для работы с графами из решения задачи 1 на представление графа
// в GraphBase добавлена функция inputGraph, которая осуществляет чтение графа
struct IGraph {
    virtual ~IGraph() {}

    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to)  = 0;

    virtual int VerticesCount() = 0;

    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) = 0;
    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) = 0;
};

// copyGraph осуществляет базовое копирование другого графа
// при этом можно легко переконвертировать граф одного типа в другой

class GraphBase: public IGraph {
protected:
    int number = 0;
public:
    GraphBase() = default;
    const void copyGraph(IGraph *Base)
    {
        for(int i = 0; i < Base->VerticesCount(); i++)
        {
            vector <int> vertices;
            Base->GetNextVertices(i, vertices);
            for(int j = 0; j < vertices.size(); j++)
            {
                this -> AddEdge(i, vertices[j]);
            }
        }
    }

    int VerticesCount() override
    {
        return number;
    }

    virtual void inputGraph(int edges)
    {
        for(int i = 0; i < edges; i++)
        {
            int from = 0, to = 0;
            cin >> from >> to;
            this -> AddEdge(from, to);
        }
    }
};

class CMatrixGraph: public GraphBase {
private:
    vector <vector <bool> > graph;
public:
    CMatrixGraph(int size)
    {
        initializeEmptyObject(size);
    }

    CMatrixGraph(IGraph *Base)
    {
        initializeEmptyObject(Base -> VerticesCount());
        copyGraph(Base);
    }
    // 1 в ячейке в строке from и столбце to означает существование ребра из вершины from в верщину to
    void AddEdge(int from, int to) override
    {
        graph[from][to] = true;
    }

    void GetNextVertices(int vertex, std::vector<int>& vertices) override
    {
        vertices.resize(0);
        for(int i = 0; i < number; i++)
            if(graph[vertex][i])
                vertices.push_back(i);
    }
    void GetPrevVertices(int vertex, std::vector<int>& vertices) override
    {
        vertices.resize(0);
        for(int i = 0; i < number; i++)
            if(graph[i][vertex])
                vertices.push_back(i);
    }
private:
    void initializeEmptyObject(int size)
    {
        this -> graph.resize(size);
        for(int i = 0; i < size; i++)
            graph[i].resize(size, false);
        this -> number = size;
    }

};

class CListGraph: public GraphBase {
private:
    vector <vector <int> > graph, graphT;
public:
    CListGraph(int size)
    {
        initializeEmptyObject(size);
    }
    CListGraph(IGraph *Base)
    {
        // *this - очень важное место, так как без него объект, созданный в CListGraph удаляется(потрачено много времени на дебаг)
        initializeEmptyObject(Base->VerticesCount());
        copyGraph(Base);
    }

    void AddVertex(void)
    {
        number++;
        vector <int> ver;
        graph.push_back(ver);
        graphT.push_back(ver);
    }

    void AddEdge(int from, int to) override
    {
        graph[from].push_back(to);
        graphT[to].push_back(from);
    }

    void GetNextVertices(int vertex, std::vector<int>& vertices) override
    {
        vertices.resize(graph[vertex].size());
        for(int i = 0; i < vertices.size(); i++)
            vertices[i] = graph[vertex][i];
    }
    void GetPrevVertices(int vertex, std::vector<int>& vertices) override
    {
        vertices.resize(graphT[vertex].size());
        for(int i = 0; i < vertices.size(); i++)
            vertices[i] = graphT[vertex][i];
    }
private:
    void initializeEmptyObject(int size)
    {
        this -> graph.resize(size);
        this -> graphT.resize(size);
        this -> number = size;
    }
};

class CSetGraph: public GraphBase {
private:
    vector <set <int> > graph, graphT;

    void initializeEmptyObject(int size)
    {
        this -> graph.resize(size);
        this -> graphT.resize(size);
        this -> number = size;
    }
public:
    CSetGraph(int size)
    {
        initializeEmptyObject(size);
    }
    CSetGraph(IGraph *Base)
    {
        // *this - очень важное место, так как без него объект, созданный в CListGraph удаляется(потрачено много времени на дебаг)
        initializeEmptyObject(Base->VerticesCount());
        copyGraph(Base);
    }

    void AddVertex(void)
    {
        number++;
        set <int> ver;
        graph.push_back(ver);
        graphT.push_back(ver);
    }

    void AddEdge(int from, int to) override
    {
        graph[from].insert(to);
        graphT[to].insert(from);
    }

    void GetNextVertices(int vertex, std::vector<int>& vertices) override
    {
        vertices.resize(graph[vertex].size());
        int j = 0;
        for(set <int>::iterator it = graph[vertex].begin(); it != graph[vertex].end(); it++)
            vertices[j++] = *it;
    }
    void GetPrevVertices(int vertex, std::vector<int>& vertices) override
    {
        vertices.resize(graphT[vertex].size());
        int j = 0;
        for(set <int>::iterator it = graphT[vertex].begin(); it != graphT[vertex].end(); it++)
            vertices[j++] = *it;
    }
};

class CArcGraph: public GraphBase {
private:
    vector < pair <int, int> > graph, graphT;
public:
    CArcGraph(int size)
    {
        initializeEmptyObject(size);
    }
    CArcGraph(IGraph *Base)
    {
        initializeEmptyObject(size);
        copyGraph(Base);
    }

    void AddVertex(void)
    {
        number++;
    }

    void swap(pair <int, int> &a, pair <int, int> &b)
    {
        pair <int, int> temp = a;
        a = b;
        b = temp;
    }
    // вытраивает вершины по порядку возрастания первой компоненты
    void insert(vector < pair <int, int> > &graf, pair <int, int> edge) {
        graf.push_back(edge);
        int last = graf.size() - 2;
        while (last > -1)
        {
            if(graf[last].first <= graf[last + 1].first) break;
            swap(graf[last], graf[last + 1]);
            last--;
        }
    }

    void AddEdge(int from, int to) override
    {
        insert(graph, make_pair(from, to));
        insert(graphT, make_pair(to, from));
    }

    int bin_search(vector < pair <int, int> > graf, int ver)
    {
        int left = 0, end = graf.size() - 1;
        while(left < end)
        {
            int mid = (left + end) / 2;

            if(ver > graf[mid].first)
                left = mid + 1;
            else
                end = mid;
        }
        return left;
    }

    void GetNextVertices(int vertex, std::vector<int>& vertices) override
    {
        vertices.resize(0);
        int start = bin_search(graph, vertex);
        while(start < graph.size() && graph[start].first == vertex)
        {
            vertices.push_back(graph[start].second);
            start++;
        }
    }
    void GetPrevVertices(int vertex, std::vector<int>& vertices) override
    {
        vertices.resize(0);
        int start = bin_search(graphT, vertex);
        while(start < graphT.size() && graphT[start].first == vertex)
        {
            vertices.push_back(graphT[start].second);
            start++;
        }
    }
private:
    void initializeEmptyObject(int size)
    {
        this -> number = size;
    }
};
// конец множества
#define MYGRAPHSOLUTION_GRAPH_H

#endif //PROJECT1_GRAPH_H
