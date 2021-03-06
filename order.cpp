//26/08/2019 (synthetic data |V|=6, youtube_ungraph.txt)
//13/11/2019 sorted version. For pathPLL specially.
#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<math.h>
#include<string>
#include<fstream>
#include<sstream>
#include<algorithm>
#include<stdint.h>
#include <bits/stdc++.h>

#define originFile "data.txt"
#define processedFile "data_pro.txt"

using namespace std;

struct degree{
    int v;
    int d;
};

bool comp(const degree &a, const degree &b){
    return a.d>b.d;
}

void cleanData();

int main()
{
    cleanData();
    return 0;
}

void cleanData(){
    vector<vector<int> > adjList;
    int countVertex;
    long countEdge;

    ifstream fin(originFile);
    stringstream ss;

    long a, b;
    string lineC;
    countVertex = 0;
    countEdge = 0;
    //long idsize = 1631813797;
    //int* id= new int[idsize];
    //memset(id,-1,idsize*sizeof(int));
    map<long,int> id;
    while(getline(fin,lineC)){
        if(lineC[0]<'0' && lineC[0]>'9') continue;

        ss << lineC;
        ss >> a;
        if(id.count(a)==0) {
            vector<int> temp1;
            adjList.push_back(temp1);
            id[a] = countVertex++;
        }

        ss >> b;
        if(id.count(b)==0) {
            vector<int> temp1;
            adjList.push_back(temp1);
            id[b] = countVertex++;
        }

        ss.clear();
        ss.str("");
        adjList[id[a]].push_back(id[b]);
        adjList[id[b]].push_back(id[a]);
    }
    fin.close();
    cout << "vertices: " << countVertex << endl;
    id.clear();

    degree temp;
    vector<degree> degreeList(countVertex,temp);
    //although for our method there is no need to sort, we preprocess the data for comparison with pathPLL.
    for(int i=0;i<countVertex;i++)
    {
        //remove duplicate edges
        sort(adjList[i].begin(),adjList[i].end());
        adjList[i].erase(unique(adjList[i].begin(),adjList[i].end()),adjList[i].end());
        countEdge += adjList[i].size();

        degreeList[i].v = i;
        degreeList[i].d = adjList[i].size();
    }
    //sort all vertices in descending order according to their degrees
    sort(degreeList.begin(),degreeList.end(),comp);
    cout << "edges:" << countEdge << endl;

    for(int i=0;i<countVertex;i++)
    {
        id[degreeList[i].v] = i;
    }

    ofstream graphFile(processedFile);
    graphFile << countVertex << " " << countEdge << endl;
    for(int i=0;i<countVertex;i++)
    {
        a = degreeList[i].v;
        for(int j=0;j<adjList[a].size();j++)
        {
            adjList[a][j] = id[adjList[a][j]];
        }
        sort(adjList[a].begin(),adjList[a].end());


        graphFile << i << " " << adjList[a].size() << " ";
        for(int j=0; j<adjList[a].size();j++) {
            graphFile << adjList[a][j] << " ";
        }
        graphFile << endl;
    }
    graphFile.close();

}
