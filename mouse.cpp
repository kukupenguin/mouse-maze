#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <string.h>

#define MAP(i,j)  map[i*col+j]  // for easy coding

using namespace std;
class Map{
    private:
    	int col, row;
    	char *map;
    public:
    	Map(int n):col(n),row(n){  // constructor
        	map = new char[row*col];
    	}
    	~Map(){ delete [] map; }
    	void putInToMap(int i, string str);
    	void setRow(int n)  { row=n;} //decided by row of input file
     	void setBoundary();
     	void printMap();
	 	void solveMap();  //找路徑 
		void printPath();  //印路徑 
		bool searchPath(){ return MAP((row-2),(col-2))=='X'; }  //看有沒有走到終點 
};
class Stack;
class Stack_item{
	private:
		int X, Y, Dir;
	friend Stack;
};
class offsets {
	public:
		int x, y;
};

class Stack{
	private:
		Stack_item *itemArray;
		int capacity;		
		int top;  
	public:
		Stack(){
			capacity = 5; // array 的空間大小
    		top = -1; // top初始值為-1
    		itemArray = new Stack_item[capacity]; //之後就可以用 itemArray[i].X 的形式來存取要存的資料
		}
		~Stack(){ delete []itemArray; }  //解構子 
		void Push( int x, int y, int d);
		void Pop(int &x , int &y , int &d);   //利用參考來回傳 x , y , d 的值
		bool IsFull(){ return top+1 == capacity; }
		bool IsEmpty() { return top == -1; }			
};

int main(void){
	
	ifstream infile("map1.txt");  // open file "tt4.txt" for read
    string str;
    if (!infile) //   if (infile.fail() )
    {       cout << "map1.txt" << "檔案無法開啟!\n" ;      
         exit(1); //exit(1)函式作用為強迫結束程式
    }
    
    getline(infile, str);
    Map m1(str.length()+2); // length() get the number of column
     
    int i=1;
    m1.putInToMap(i++,str);
    for ( ; getline(infile, str) && str.length()!=0; i++)
    	m1.putInToMap(i,str);

    m1.setRow(i+1);  // set the number of map row
    m1.setBoundary();  // set '0' to map boundary
    m1.printMap();   // print map for testing
	m1.solveMap();  //找出路徑 
	
    if( m1.searchPath() )  //有找到路 
        m1.printPath();  //印出X路徑 
    else  //沒找到路 
    	cout << "no path found!\n";

    infile.clear();  infile.close();  
     
	system("pause");
  	return(0);
}
void Stack::Push(int x, int y, int d){
	if(IsFull()) {
        capacity*=2;   // 把空間變大
        Stack_item *temp=new Stack_item[capacity] ;//宣告一個temp去接原本存的內容
        copy(itemArray,itemArray+top+1,temp) ;
        delete [] itemArray ; //刪掉舊家
        itemArray = temp ;  //搬到新家
 	}
 	itemArray[++top].X=x;
 	itemArray[top].Y=y;
 	itemArray[top].Dir=d;			
}
void Stack::Pop(int &x , int &y , int &d) {  //利用參考來回傳 x , y , d 的值
	if(!IsEmpty()){
		x=itemArray[top].X;
		y=itemArray[top].Y;
		d=itemArray[top--].Dir;
	}
}
void Map::putInToMap(int i, string str) {       
    if(i>=row-1) {  // if out of space, enlarge
        char *map_temp = new char[row*col*2];    
        for(int k=0; k<row*col ;k++ )
             map_temp[k]=map[k];
        delete []map;
        map= map_temp;
        row *=2;
    }  
    for (int j=1; j<col-1; j++)
        MAP(i,j) =str[j-1];       
}

void Map::setBoundary() {
    for(int i=0; i< row; i++)
        map[i]=map[(row-1)*col+i]=map[i*col]=map[i*col+(col-1)]='0';
}

void Map::printMap () {
    for(int i=0; i< row; i++){
        for (int j=0; j<col; j++)
            cout<< MAP(i,j);      
        cout<< "\n";     
    }
}
 
void Map::solveMap() {

	int x=0, y=0, d=0;
	offsets offs[ 4 ] ;  //4個方向 
	offs[0].x = 1 ; 
	offs[0].y = 0 ;
	offs[1].x = 0 ; 
	offs[1].y = 1 ;
	offs[2].x = -1 ; 
	offs[2].y = 0 ;
	offs[3].x = 0 ; 
	offs[3].y = -1 ; 

	Stack stk ;
	stk.Push( 1 , 1 , -1 ) ;
	do {
		stk.Pop ( x , y , d ) ; // void Pop(int &x , int &y , int &d) 
		for (int i = d + 1 ; i < 4 ; i++ )
		{
			 if ( MAP( (y + offs[i].y) , (x + offs[i].x) ) == '1' ) {  //y,x有路可走 
				stk.Push(x, y, i);   
				MAP(y,x) = '0' ;  //避免重走 
				x = x + offs[i].x; y = y + offs[i].y; d = -1;
				i = d;  //讓i從0開始跑 
				if (  y == (row-2) && x == (col-2) ){  //判斷到終點 
					stk.Push(x, y, i);
					i = 4;  //跳出loop 
				}
			}
		}
	}while( ( y!=(row-2) || x!=(col-2) ) && !stk.IsEmpty() );  //到終點或堆疊為空跳出loop 

	while( !stk.IsEmpty( ) ) //如果stack還沒空 就一直執行
	{
		stk.Pop(x, y, d);
		MAP(y,x)='X';  //標出路徑 
	}
}
 //只印出路徑 
void Map::printPath(){
	for(int i=0;i<row;i++){
		for(int j=0;j<col;j++){
			if(MAP(i,j)=='X')
				cout<<'X';
			else  
				cout<<' ';
		}
		cout<<endl;
	}
}
