//n-皇后问题的随机搜索算法，找到解即可
//用一维数组来存放每一列皇后的行号
//30w/1min
/* 
Author:HenRiser
email:lhrc22111606@mail.ustc.edu.cn
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int *solution; //全局变量，每列一个皇后，solution[col]=row,表示第col列的皇后在row行
int len;            //全局变量，皇后数量，len*len的棋盘
int lenDiag;        //全局变量，对角线数量，2*len-1
int *MainDiag;  //主对角线方向上的皇后数，MainDiag[i]代表第主对角线方向上下标为i的对角线上的皇后数量，规定左下开始为0
int *SubDiag;   //次对角线方向上的皇后数，SubDiag[i]代表第次对角线方向上下标为i的对角线上的皇后数量，规定左上开始为0
int value = 0;  //全局变量保存现在的分数
int i,j;
//函数声明
void initSolution();			//  初始化一个棋盘布局,将邻居数组准备好
void initDiag();                //  初始化对角线方向上皇后数量0
void restart();                 //  重开函数，扰动现有布局
void swap(int , int);           //  将棋盘的第i列和第j列交换  
void traverseNeighbors();        //  返回新的最小评估值，若没有更小的评估值则返回-1
int evaluate();                 //  主次对角线方向上的互相攻击的皇后对数之和即为其该棋盘分数
void debug();                   //  输出结果
int newEva(int, int);           //  返回交换i,j后冲突对数的变化
void swapDiag(int,int);         //  更新对角线上的皇后信息
void BigSwap(int,int);          //  变换行列与对角线
int NqueenTest(int* , int );    //  传入N皇后的数组(从0开始存)以及N的大小

int main(){
	printf("请输入皇后个数:");
    scanf("%d",&len);     	// 键盘读入棋盘的大小
    lenDiag = 2*len - 1;    //对角线数量
	solution = (int *)malloc(sizeof(int)*len);	    if (!solution)  return 0; //分配空间失败
    SubDiag =  (int *)malloc(sizeof(int)*lenDiag);	if (!SubDiag)   return 0; //分配空间失败
    MainDiag = (int *)malloc(sizeof(int)*lenDiag);	if (!MainDiag)  return 0; //分配空间失败
    long seed = clock();  //随机数种子，如果这个数不改变，那么每次算法运行的结果都是一样的，即使用rand()函数
	initSolution();
    initDiag();//遍历赋值对角线上皇后数
    printf("init finished\n");
    value = evaluate();//当前冲突对数
	while (value)	
        traverseNeighbors();
    int end = clock();
	// printf("N = %d,冲突数为:0,%d\n得到的解为:",len,value);
	// for(i=0;i<len;++i)	printf("%d,",solution[i]);
    printf("\nn = %d cost %d ms\n",len,(end-seed));
    int test = NqueenTest(solution,len);
	free(solution);free(SubDiag);free(MainDiag);
	return 0;
}

void swap(int i, int j){//done
	int temp=solution[i];
    solution[i]=solution[j];
    solution[j]=temp;
}

void initSolution(){
	for(i=0;i<len;++i)     
     	solution[i]=i;         //皇后都在主对角线上，之后进行swap操作也能保证皇后们只有对角线这一攻击可能
	for(i = 0; i < len/2; ++i)  swap(i,rand()%len);	// 对棋盘进行2*len次随机交换
}

void initDiag(){    
    for(i=0;i<lenDiag;++i){//对角线初始化
        MainDiag[i]=0;
        SubDiag[i]=0;
    }
}

/***  检查当前解（棋盘）的邻居棋盘（交换solution[i]和solution[j],i和j随机选择，共有n(n-1)/2种不同的可能）
 ***  保留冲突最小的棋盘，将它视为新的解（best found策略）
 ***  若返回-1,表示没有找到更好的邻居，需要重启 restart() ,返回0表示找到所求的解，
 ***  返回大于0的值表示需要更新当前解solution后要继续遍历新解的邻居
 ***/
void traverseNeighbors(){    
    int neweva,min_ChangedPairs = 0;//如果找不到冲突对数的改变小于0的情况则重开
    for(i=0;i<len;i++)  {//遍历邻居，评估每个邻居冲突皇后对的个数，找到最小冲突对数的邻居
        if(MainDiag[len-1+i-solution[i]]>1||SubDiag[i+solution[i]] >1)
            for(j=0;j<len;j++){
                if(i!=j){//两个不同皇后
                    neweva = newEva(i,j);
                    if(i-solution[i] == j-solution[j] || i+solution[i] == j+solution[j]) //在对角线时按原算法会少2个冲突
                        neweva+=2;
                    if(neweva<0){   //该过程未改变棋盘布局
                        min_ChangedPairs = neweva; //能减去的最小分数
                        value += min_ChangedPairs;
                        BigSwap(i,j);// 找到更好的移动方式，那么就以这个邻居进行下一次爬山
                        break;
                    }
                }
            }
    }
    if (min_ChangedPairs == 0) //如果遍历了所有的邻居节点，也找不到更好的移动方式，那么就需要重新启动算法，初始棋局改变一下
        restart();//要重开了
}

/***  当遍历完所有邻居，也没有找到更好的邻居
 ***  那么随机重置solution，然后再用爬山法搜索
 ***  随机重启:邻居是邻域内的“微小”的扰动，增加扰动范围，比如随机交换times=20次
 ***/
void restart(){
	int times = 5;  // 随机交换解的次数，扰动大小,设置times>1即可
	for(int i = 0; i < times; ++i)	swap(rand()%len,rand()%len);
	puts("restart()!");//重开！
}

/***  计算棋盘的评价函数（棋盘的价值: value=可相互攻击到的皇后对数）
 ***  当皇后相互攻击不到时，目标函数的值value==0
 ***   solution[i]==solution[j], (solution[i]-solution[j])/(i-j)==1或-1时（C语言实现会有bug），两个皇后彼此能攻击到，value++
 ***  棋盘编码方式能保证不会出现 solution[i]==solution[j]，故该条件可以不用判断
 ***  思考:该函数的时间性能是否能提高？
 ***/
int evaluate(){
    int pairs = 0;
    for(i = 0;i<len;i++){//遍历每个皇后所在
        MainDiag[len-1+i-solution[i]]++;//下标为i的皇后所在主对角线方向上对角线对应加1
        SubDiag[i+solution[i]]++;//下标为i的皇后所在次对角线方向上对角线对应加1
        if(MainDiag[len-1+i-solution[i]]>1)   pairs +=MainDiag[len-1+i-solution[i]]-1;
        if(SubDiag[i+solution[i]] >1)         pairs +=SubDiag[i+solution[i]]       -1;
    }   //增加冲突对数
    return pairs;
}

void debug(){
    printf("\nnow debug,eva = %d ,value = %d\n",evaluate(),value);
}

int newEva(int i,int j){
    int changedPairs = 0;   
    if(MainDiag[len-1+i-solution[j]]>0) changedPairs +=MainDiag[len-1+i-solution[j]];    //如果改动后的皇后对角线上
    if(MainDiag[len-1+j-solution[i]]>0) changedPairs +=MainDiag[len-1+j-solution[i]];    //有其他皇后，即有冲突存在
    if(SubDiag [i+solution[j]]      >0) changedPairs +=SubDiag [i+solution[j]];          //增加冲突即加上该皇后
    if(SubDiag [j+solution[i]]      >0) changedPairs +=SubDiag [j+solution[i]];    
    if(MainDiag[len-1+i-solution[i]]>1) changedPairs -=MainDiag[len-1+i-solution[i]]-1;    //如果改动前的皇后对角线上
    if(MainDiag[len-1+j-solution[j]]>1) changedPairs -=MainDiag[len-1+j-solution[j]]-1;    //有其他皇后，即有冲突存在
    if(SubDiag [i+solution[i]]      >1) changedPairs -=SubDiag [i+solution[i]]-1;          //化解冲突即减去该皇后
    if(SubDiag [j+solution[j]]      >1) changedPairs -=SubDiag [j+solution[j]]-1; 
    return changedPairs;
}

void swapDiag(int i,int j){
    MainDiag[len-1+i-solution[i]]--;//皇后要离开的位置
    MainDiag[len-1+j-solution[j]]--;//减少其对角线上的
    SubDiag [i+solution[i]]      --;//皇后数量
    SubDiag [j+solution[j]]      --;
    MainDiag[len-1+i-solution[j]]++;//皇后要降临的位置
    MainDiag[len-1+j-solution[i]]++;//增加其对角线上的
    SubDiag [i+solution[j]]      ++;//皇后数量
    SubDiag [j+solution[i]]      ++;
}

void BigSwap(int i,int j){//变换行列与对角线
    swapDiag(i,j);
    swap(i,j);
}

int NqueenTest(int* nqueen_array, int length)		//传入N皇后的数组(从0开始存)以及N的大小
{
	int size_set = 10;
	if (length <= 4) {
		printf("棋盘太小！\n");
		return -4;
	}
	if (length <= size_set) {			//小棋盘二维打印
		for (int i = 0; i < length; i++) {
			for (int j = 0; j < length; j++) {
				if (j == nqueen_array[i]) {
					printf("* ");
				}
				else {
					printf("0 ");
				}
			}
			printf("\n");
		}
	}
	else {
		for (int i = 0; i < length; i++) {
			// printf("%d\n", nqueen_array[i]);
		}
	}
	for (int i = 0; i < length; i++) {
		if (nqueen_array[i] < 0 || nqueen_array[i] >= length) {
			printf("皇后摆出棋盘了\n");
			return -3;
		}
		for (int j = i + 1; j < length; j++) {
			if (nqueen_array[i] == nqueen_array[j]) {
				printf("直线冲突\n");
				return -2;
			}
			if (abs(nqueen_array[i] - nqueen_array[j]) == abs(i - j)) {		//abs需要include<stdlib.h>
				printf("斜线冲突\n");
				return -1;
			}
		}
	}
	printf("N皇后的解正确\n");
	return 1;
}