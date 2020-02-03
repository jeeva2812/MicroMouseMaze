#include <bits/stdc++.h>

using namespace std;

int pos_x = 0, pos_y = 0;
int n=6;

stack <int> s;
vector <bool> vis(n*n + 1, false);

int dec(int x,int y){
    return n*x + y;
}

void dfs(int x,int y){

    pos = dec(x,y);

    if((x==n/2 || x==(n/2-1)) && (y==n/2 || y==(n/2-1))) return;
    vis[pos] = true;
    s.push(pos);

}

int main(){

    dfs(pos_x,pos_y);

    return 0;
}