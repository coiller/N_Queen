#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;

const double C1 = 0.45;
const double C2 = 32;

void initQueen(int N , vector <int> &queen) // 初始化Queen（非同行非同列）
{
    vector <int> array(N);
    for(int i = 0; i < N ; i++)
        array[i] = i;//生成0～N-1的序列
    queen.clear();
    while(N)//每一列
    {
        double t = rand()%N;
        queen.push_back(array[t]);//Queen不在同一行
        array[t] = array[--N];
    }
}

long long compute_collisions(const vector<int> &queen,vector<int> &dn,vector<int> &dp)//计算每个对角线上皇后的个数，dp[]和dn[]，并计算总的碰撞次数collisions
{
    for(int t =0 ; t < 2*queen.size() ; t++)
    {
        dn[t] = dp[t] = 0;
    }

    for(int i = 0 ; i < (int)queen.size() ; i++)
    {
        int j = queen.at(i);
        dn[i+j]++;
        dp[queen.size()-1+i-j]++;
    }

    long long collisions = 0;
    for(int k =0 ; k < 2*queen.size()-1;k++)
    {
        collisions +=dn[k]*(dn[k]-1)/2;
        collisions +=dp[k]*(dp[k]-1)/2;
    }
    return collisions;
}

int compute_attacks(const vector<int> &queen,vector<int> &dn,vector<int> &dp,vector<int>&attack)//计算被攻击的皇后的个数
{
    attack.clear();
    int attackedLines = 0;
    for(int i =0 ; i<(int)queen.size();i++)
    {
        int j = queen.at(i);
        if(dn.at(i+j) > 1 || dp.at(queen.size()-1+i-j) > 1)
        {
            attack.push_back(i);
            attackedLines++;
        }
    }
    return attackedLines;
}

int randInt(int k ,int size)//找不重复的位置
{
    int val =rand()%size;
    while(val == k)
        val = rand()%size;

    return val;
}


int cacl_collisions(const vector<int> &queen,vector<int> &dn,vector<int> &dp,int a,int b)//计算互换后冲突的改变量
{
    int i,j,counts = 0 ;

    i = queen[a];
    j = queen[b];

    counts -= dn[a+i] - 1;
    counts -= dn[b+j] - 1;
    if(a+i == b+j) counts++;

    counts -= dp[queen.size()-1+a-i] - 1;
    counts -= dp[queen.size()-1+b-j] - 1;
    if(a-i == b-j) counts++;

    counts += dn[a+j] ;
    counts += dn[b+i] ;
    if(a+j == b+i) counts++;

    counts += dp[queen.size()-1+a-j] ;
    counts += dp[queen.size()-1+b-i] ;
    if(a-j == b-i) counts++;

    return counts;
}

void updateQueen(vector<int> &queen,vector<int> &dn,vector<int> &dp,int a,int b)//更新交换后的棋盘信息
{
    int i = queen.at(a);
    int j = queen.at(b);

    dn[a+i]--;
    dn[b+j]--;
    dp[queen.size()-1+a-i]--;
    dp[queen.size()-1+b-j]--;

    int t = queen[a];
    queen[a] = queen[b];
    queen[b] = t;

    dn[a+j]++;
    dn[b+i]++;
    dp[queen.size()-1+a-j]++;
    dp[queen.size()-1+b-i]++;
}

void printQueen(vector<int>&queen)//打印棋盘
{
    for(int i = 0 ; i < (int)queen.size(); i++)
    {
        for(int j = 0;j < (int)queen.size();j++)
        {
            if(j == queen.at(i))
                cout<<"0";
            else cout<<"*";
        }
        cout<<endl;
    }
}
void qs(int N)//quick search
{
    long long collisions = N;
    int num_of_attack;
    int swapCount = 0;

    vector<int> queen;
    vector<int> attack;

    vector<int> dn(2*N);
    vector<int> dp(2*N);

    if(N == 2||N == 3)
    {
        cout<<"无解"<<endl;
        return ;
    }

    while(collisions)
    {
        initQueen(N,queen);
        collisions = compute_collisions(queen, dn, dp);
        num_of_attack = compute_attacks(queen, dn, dp, attack);

        int limit = C1*collisions;
        int loopSteps = 0;
        while(loopSteps < C2*N)
        {
            for(int k = 0 ; k < num_of_attack ; k++)
            {
                int i = attack.at(k);
                int j = randInt(i,N);

                int c;
                if(( c = cacl_collisions(queen,dn,dp,i,j)) < 0)
                {
                    collisions += c;
                    updateQueen(queen,dn,dp,i,j);
                    swapCount++;

                    if(collisions == 0)break;
                    if(collisions < limit)
                    {
                        limit = C1*collisions;
//                      k = 0;
                        num_of_attack = compute_attacks(queen, dn, dp, attack);
                    }
                }
            }
            if(collisions == 0)break;
            loopSteps +=num_of_attack;
        }
    }
    cout<<"交换次数: "<<swapCount<<endl;
  printQueen(queen);
}

int main()
{
    int N=100000;
    clock_t startTime,endTime;

    cout<<"请输入皇后的个数：";
    cin>>N;

    srand((unsigned)time(NULL));

    startTime = clock();
    qs(N);
    endTime = clock();

    double totalTime = 1.0*(endTime - startTime)/CLOCKS_PER_SEC;
    cout<<"共耗时："<<totalTime<<"s"<<endl;

    return 0;
}
