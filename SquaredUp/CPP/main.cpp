#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <queue>
#include <stack>

using namespace std;

class VertexOfGraph {       //定义图的节点类
public:
    int vertex[3][3];
    int h,g;
    int x,y;
    string hashValue;

public:
    VertexOfGraph() {       //构造函数将成员变量初始化为0和空
        x = 0;
        y = 0;
        h = 0;
        g = 0;
        hashValue = "";
    };
    bool isValid() {        //判断当前状态是否有效,如果超出边界范围则无效
        if(x >= 0 && x < 3 && y >= 0 && y < 3) {
            return true;
        }
        return false;
    }
    void newStatus(int oldX, int oldY) {    //将现在存储的空位位置x,y与oldX,oldY对应的位置交换数值,得到新状态
        int temp = vertex[x][y];
        vertex[x][y] = vertex[oldX][oldY];
        vertex[oldX][oldY] = temp;
    }
};

VertexOfGraph createGraph(int vertex[3][3]);
string getHash(int v[3][3]);
map<string,string> bfsSearch(VertexOfGraph,string,map<string,bool>);
void display(map<string,string>, string);

int main()
{
    int origin[3][3],target[3][3];
    /*
     * visited判断当前状态是否访问过,若已访问过为true,否则false
     * 每个字符串9个字节,bool一个字节,共10个字节, 9!*10/1024/1024=3.5M
     */
    map<string,bool> visited;
    map<string,string> searchRoute;                     //记录搜索路径中每个状态的hash值
    VertexOfGraph graph;

    //输入初始状态
    cout << "Please input the original status(请输入初始状态):" << endl;
    for(int i=0; i < 3; i++) {
        for(int j = 0; j <3; j++) {
            cin >> origin[i][j];
        }
    }
    //输入目标状态
    cout << "Please input the target status(请输入目标状态):" << endl;
    for(int i=0; i < 3; i++) {
        for(int j = 0; j <3; j++) {
            cin >> target[i][j];
        }
    }
    //获取目标状态的hash值
    string targetHash = getHash(target);
    //创建图结构
    graph = createGraph(origin);
    //cout << graph.hashValue << targetHash;            //测试是否成功把整个状态转换为字符串保存,作为整个状态的hash值

    if(graph.hashValue == targetHash) {                 //如果起始状态就是目标状态
        cout << "The origin status is same as the target status, so you do not need to move.\n";
    } else {
        visited[graph.hashValue] = true;
        searchRoute = bfsSearch(graph,targetHash,visited);  //通过广度优先搜索获取到达目标状态的路径

        display(searchRoute, targetHash);
    }

    return 0;
}

string getHash(int v[3][3]) {                           //将状态的数字排序转换为字符串,作为hash值
    string hashString = "";
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            stringstream sstream;
            string str;
            sstream << v[i][j];                          //注意sstream输入是<<而不是iostream的>>
            sstream >> str;
            hashString += str;
        }
    }
    return hashString;
}

VertexOfGraph createGraph(int vertex[3][3]) {
    VertexOfGraph returnGraph;
    for(int i=0; i < 3; i++) {
        for(int j = 0; j <3; j++) {
            returnGraph.vertex[i][j] = vertex[i][j];
            if(vertex[i][j] == 0) {
                returnGraph.x = i;
                returnGraph.y = j;
            }
        }
    }
    returnGraph.hashValue = getHash(vertex);

    return returnGraph;
}

map<string,string> bfsSearch(VertexOfGraph graph,string targetHash,map<string,bool> visited) {
    int direction[4][2] = { {-1,0}, {0,1}, {1,0}, {0,-1} };
    queue<VertexOfGraph> que;
    que.push(graph);
    map<string,string> route;

    while(!que.empty()) {
        VertexOfGraph movingVertex = que.front();
        //cout << movingVertex.hashValue << endl;       //测试遍历结果
        VertexOfGraph savingVertex = movingVertex;
        que.pop();

        for(int i = 0; i < 4; i++) {
            movingVertex = savingVertex;
            movingVertex.x = savingVertex.x + direction[i][0];
            movingVertex.y = savingVertex.y + direction[i][1];
            if(!movingVertex.isValid()) {
                continue;
            } else {
                movingVertex.newStatus(savingVertex.x, savingVertex.y);
                movingVertex.hashValue = getHash(movingVertex.vertex);
                if(visited.find(movingVertex.hashValue)==visited.end()) {
                    visited[movingVertex.hashValue] = true;
                    route[movingVertex.hashValue] = savingVertex.hashValue;

                    que.push(movingVertex);
                }
                if(movingVertex.hashValue == targetHash) {
                    return route;
                }
            }
        }
    }
    return route;
}

void display(map<string,string> searchRoute,string targetHash) {
    stack<string> shortestRoute;
    shortestRoute.push(targetHash);

    string nextWay = targetHash;
    while( searchRoute.find(nextWay) != searchRoute.end() ) {
        nextWay = searchRoute[nextWay];
        shortestRoute.push(nextWay);
    }

    cout << endl;
    int counts = 0;
    while( !shortestRoute.empty() ) {
        string displayString = shortestRoute.top();
        shortestRoute.pop();

        if( (counts == 0) && (displayString == targetHash) ) {       //如果堆栈内只有一个字符串元素,且等于targetHash,那么说明遍历了所有的状态也无法达到目标状态
            cout << "Sorry, but you can not reach the target status, and you can try another status instead." << endl;
            cout << "无法到达目标状态." << endl;
            break;
        }

        cout << "Step " << counts << " :";
        counts++;
        for(unsigned int i = 0; i < displayString.length(); i++) {
            if( i % 3 == 0 ) {
                cout << endl;
            }
            cout << displayString.substr(i,1) << ' ';
        }
        cout << endl;
    }
    if( counts > 1 ) {
        cout << "Totally, you used " << counts-1 << " steps to the target status." << endl;
        cout << "共使用了" << counts - 1 << "步到达目标状态." << endl;
    }
}
