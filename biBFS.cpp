#include<iostream>
#include<vector>
#include<set>
#include<math.h>
#include<string>
#include<fstream>
#include<sstream>
#include<algorithm>
#include<stdint.h>
#include <bits/stdc++.h>

#include<sys/time.h>

#define originFile "douban_pro.txt"
using namespace std;

class biBFS{
private:
    int countVertex;
    vector<vector<int> > adjList;

    int* queueA;
    int* queueB;
    int  frontA, frontB;
    int  endA,   endB;

    int* modify;
    int  endM;
    int  sumDistance;

    int8_t* dist;
public:
    biBFS()
    {
        ifstream graphFile(originFile);

        stringstream ss;
        string lineC;

        getline(graphFile,lineC);
        ss << lineC;
        ss >> countVertex;
        ss.clear();
        ss.str("");

        int v,d;
        //read the graph
        while(getline(graphFile,lineC))
        {
            ss << lineC;
            ss >> v; //vertex id
            ss >> d; //degree
            vector<int> tempD(d,0);
            for(int i=0;i<d;i++){
                ss >> tempD[i];
            }
            adjList.push_back(tempD);
            ss.clear();
            ss.str("");
        }
        graphFile.close();
        cout << countVertex << endl;

        queueA= new int[countVertex];
        queueB= new int[countVertex];
        modify= new int[countVertex];

        dist = new int8_t[countVertex];
        memset(dist,99,countVertex);
    }

    void pathQuery(int a, int b)
    {
        memset(dist,99,countVertex*sizeof(int8_t));

        dist[a] = 1; //from 1 to 2, 3, 4...
        dist[b] = -1;//from -1 to -2, -3, -4...
        frontA = 0; //initialize the queueA by setting the front pointer to 0;
        frontB = 0; //initialize the queueB by setting the front pointer to 0;
        queueA[0] = a; //push a into queueA;
        endA = 1;
        queueB[0] = b; //push b into queueB;
        endB = 1;

        vector<int> meet; //vertices on the level where bi-BFS meets.

        int u, v;
        int p; //a variable
        //part A: bi-BFS search on the sparsifed graph
        int level = 0;
        while(meet.empty() && level<50) //the level-bounded bi-BFS (based on the upper bound)
        {
            level++;
            if(endA-frontA<=endB-frontB) //the size of queueA is smaller than the size of queueB, we search from side a.
            {
                p = endA; //the end of this level
                while(frontA<p)
                {
                    u = queueA[frontA++];
                    for(int i=0;i<adjList[u].size();i++)
                    {
                        v = adjList[u][i];
                        if(dist[v]==99) //v is unvisited before
                        {
                            dist[v] = dist[u] + 1;
                            queueA[endA++] = v; //push v into next level of queueA
                        }
                        else if(dist[v]<0) //v has been visited from side b
                        {
                            meet.push_back(v); //v is on the level where the bi-BFS meets
                            dist[v] = dist[u] + 1; //set dist[v]>0 for consistency
                        }
                    }
                }
            }
            else //search from side b.
            {
                p = endB; //the end of this level
                while(frontB<p)
                {
                    u = queueB[frontB++];
                    for(int i=0;i<adjList[u].size();i++)
                    {
                        v = adjList[u][i];
                        if(dist[v]==99) //v is unvisited before
                        {
                            dist[v] = dist[u] - 1;
                            queueB[endB++] = v;
                        }
                        else if(dist[v]>0) //v has been visited from side a
                        {
                            meet.push_back(v); //v is on the level where the bi-BFS meets
                        }   //keep dist[v]>0 for consistency
                    }
                }
            }
        }

        if(meet.size()!=0) //if the MEET is not empty, there exist SPs on G'
        {
            p = dist[meet[0]]-level-2; //for v on the meet level, its dist[] is dist[v]>0(from side a) or p<0(from side b)
            frontA = 0;
            frontB = 0;
            endA = 0;
            endB = 0;

            for(int i=0;i<meet.size();i++) //push all predecessors of v w.r.t a and b to queueA and queueB separately
            {
                u = meet[i];
                for(int j=0;j<adjList[u].size();j++)
                {
                    v = adjList[u][j];
                    if(dist[v]+1==dist[u]) //predecessor of v w.r.t a
                    {
                        //cout << u << " " << v << " ";
                        queueA[endA++] = v;
                    }
                    else if(dist[v]==p+1) //predecessors of v w.r.t b
                    {
                        //cout << u << " " << v << " ";
                        queueB[endB++] = v;
                    }
                }
            }
            while(frontA!=endA) //queueA not empty
            {
                u = queueA[frontA++];
                if(dist[u]==99) continue; //a vertex may appear in the queue more than once.
                for(int i=0;i<adjList[u].size();i++)
                {
                    v = adjList[u][i];
                    if(dist[v]+1==dist[u]) //predecessors w.r.t a
                    {
                        queueA[endA++] = v;
                        //cout << u << " " << v << " ";
                    }
                }
                dist[u] = 99; //avoid visiting a vertex twice by setting its dist=99
            }
            while(frontB!=endB)
            {
                u = queueB[frontB++];
                if(dist[u]==99) continue;
                for(int i=0;i<adjList[u].size();i++)
                {
                    v = adjList[u][i];
                    if(dist[v]==dist[u]+1)
                    {
                        queueB[endB++] = v;
                        //cout << u << " " << v << " ";
                    }
                }
                dist[u] = 99;
            }
        }
        sumDistance += level;

    }

    unsigned int rand_32()
    {
        return (rand()&0x3)<<30 | rand()<<15 | rand();
    }

    void testPath(int testTime)
    {
        int a,b;
        sumDistance = 0;
        timeval st,ed;
        gettimeofday(&st,NULL);
        for(int i=0;i<testTime;i++)
        {
            a = rand_32()%countVertex;
            b = rand_32()%countVertex;
            pathQuery(a,b);
        }
        gettimeofday(&ed,NULL);
        long t = (ed.tv_sec-st.tv_sec)*1000000 + (ed.tv_usec-st.tv_usec);
        cout << "average query time:" << t*1.0/testTime << endl;
        cout << "average distance:" << sumDistance*1.0/testTime << endl;

        /*sumDistance = 0;
        gettimeofday(&st,NULL);
        for(int i=0;i<testTime;i++)
        {
            a = rand_32()%countVertex;
            b = rand_32()%countVertex;
            pathQuery(a,b);
        }
        gettimeofday(&ed,NULL);
        t = (ed.tv_sec-st.tv_sec)*1000000 - (ed.tv_usec-st.tv_usec);
        cout << "average query time:" << t*1.0/testTime << endl;
        cout << "average distance:" << sumDistance*1.0/testTime << endl;*/
    }
};

int main()
{
    srand(1);
    biBFS test = biBFS();
    /*int a,b;
    while(true)
    {
        cin >> a;
        cin >> b;
        test.pathQuery(a,b);
    }*/

    test.testPath(10000);
    return 0;
}
