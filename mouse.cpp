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
	 	void solveMap();  //����| 
		void printPath();  //�L���| 
		bool searchPath(){ return MAP((row-2),(col-2))=='X'; }  //�ݦ��S��������I 
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
			capacity = 5; // array ���Ŷ��j�p
    		top = -1; // top��l�Ȭ�-1
    		itemArray = new Stack_item[capacity]; //����N�i�H�� itemArray[i].X ���Φ��Ӧs���n�s�����
		}
		~Stack(){ delete []itemArray; }  //�Ѻc�l 
		void Push( int x, int y, int d);
		void Pop(int &x , int &y , int &d);   //�Q�ΰѦҨӦ^�� x , y , d ����
		bool IsFull(){ return top+1 == capacity; }
		bool IsEmpty() { return top == -1; }			
};

int main(void){
	
	ifstream infile("map1.txt");  // open file "tt4.txt" for read
    string str;
    if (!infile) //   if (infile.fail() )
    {       cout << "map1.txt" << "�ɮ׵L�k�}��!\n" ;      
         exit(1); //exit(1)�禡�@�ά��j�������{��
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
	m1.solveMap();  //��X���| 
	
    if( m1.searchPath() )  //������ 
        m1.printPath();  //�L�XX���| 
    else  //�S���� 
    	cout << "no path found!\n";

    infile.clear();  infile.close();  
     
	system("pause");
  	return(0);
}
void Stack::Push(int x, int y, int d){
	if(IsFull()) {
        capacity*=2;   // ��Ŷ��ܤj
        Stack_item *temp=new Stack_item[capacity] ;//�ŧi�@��temp�h���쥻�s�����e
        copy(itemArray,itemArray+top+1,temp) ;
        delete [] itemArray ; //�R���®a
        itemArray = temp ;  //�h��s�a
 	}
 	itemArray[++top].X=x;
 	itemArray[top].Y=y;
 	itemArray[top].Dir=d;			
}
void Stack::Pop(int &x , int &y , int &d) {  //�Q�ΰѦҨӦ^�� x , y , d ����
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
	offsets offs[ 4 ] ;  //4�Ӥ�V 
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
			 if ( MAP( (y + offs[i].y) , (x + offs[i].x) ) == '1' ) {  //y,x�����i�� 
				stk.Push(x, y, i);   
				MAP(y,x) = '0' ;  //�קK���� 
				x = x + offs[i].x; y = y + offs[i].y; d = -1;
				i = d;  //��i�q0�}�l�] 
				if (  y == (row-2) && x == (col-2) ){  //�P�_����I 
					stk.Push(x, y, i);
					i = 4;  //���Xloop 
				}
			}
		}
	}while( ( y!=(row-2) || x!=(col-2) ) && !stk.IsEmpty() );  //����I�ΰ��|���Ÿ��Xloop 

	while( !stk.IsEmpty( ) ) //�p�Gstack�٨S�� �N�@������
	{
		stk.Pop(x, y, d);
		MAP(y,x)='X';  //�ХX���| 
	}
}
 //�u�L�X���| 
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
