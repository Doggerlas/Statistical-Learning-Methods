//支持四种距离
#include <iostream>
#include <iomanip>
#include <fstream>//ofstream
#include <sstream>
#include<string>
#include <vector>
#include<time.h>//time_t
#include<numeric>//accumulate 
#include<algorithm>//for_each

#define file_dir "D:\\研究生\\统计学习方法\\3.读取文本\\class" //为运行程序，请修改此处程序路径

using namespace std;


/*一列数据计算的结果*/
struct Data{
	double mean;
	double stdev;
};

vector<string> kong1;//没用，占位的
vector<Data> kong2;//没用，占位的
vector <string> clm;
vector<string> cls;
vector<double> block;
//result1-4分别用于存储class1-4的均值与方差
vector<Data> result1;
vector<Data> result2;
vector<Data> result3;
vector<Data> result4;
double Chebyshev1[31];//每个元素是|Xi-Xmean|
double Chebyshev2[31];//每个元素是|Xi-Xmean|
double Chebyshev3[31];//每个元素是|Xi-Xmean|
double Chebyshev4[31];//每个元素是|Xi-Xmean|




//每个测试样本（偶数行）到样本中心（result.mean）的距离
double dst1=0.0,dst2=0.0,dst3=0.0,dst4=0.0;
int matrix[4][4];//混淆矩阵



/**********************************
函数功能：获取程序执行时间
参数：
返回值：%Y-%m-%d %H:%M:%S
**********************************/
string getTime()
{
	time_t timep;
	time (&timep);
	char tmp[64];
	strftime(tmp,sizeof(tmp),"%Y-%m-%d %H:%M:%S",localtime(&timep));
	return tmp;
}

/**********************************
函数功能：获取文件名
参数：i:文件标识
返回值： string=file_diri.txt
**********************************/
char* getFilename(int i)
{
	char s[2];
	itoa(i,s,10);
	char sen[50];
	strcpy(sen,file_dir);
	strcat(sen,s);
	strcat(sen,".txt");
	char *p=new char[50];//分配一块具体内存，不new就是保存地址的一个指针变量
	strcpy(p, sen);
	return p;
}
/**********************************
函数功能：文本输出
参数：i文件标识          ifs文件输入句柄        inf提示信息 opt对应输出格式
返回值：输出语句
**********************************/
void Out_Info(int i,int opt,string inf,vector<string> &vec1,vector<Data> &vec2) 
{

	switch (opt)
    {
        case 1:
			printf("\n第%d份文件执行时间：%s\n",i,inf.c_str());
            break;
        case 2:
            cout << "读入"<<inf<<"的所有列数据为 " << endl;
            break;
        case 3:
            for (auto it = vec1.begin(); it != vec1.end(); it=it+33)
			{
				for(int row_num=0;row_num<33;row_num++)
				{
					cout.setf(ios::left); //设置对齐方式为left 
					cout.width(25); //设置宽度为30，不足用空格填充 
					cout << *(it+row_num);
				}
			cout<<endl;
			}
			break;
		case 4:
			cout<<"class"<<i<<"的奇数行均值与方差result"<<i<<"如下："<<endl;
			cout.fill('.'); //设置填充方式 
			cout.width(825); //设置宽度，只对下条输出有用 
			cout<<"*";
			cout<<"*";
			cout<<endl;
			break;
		case 5:
			for (unsigned c = 0; c<vec2.size(); c+=1)
			{
				if(c<9){
					cout<<"mean"<<c+1<<"=";cout.fill(' ');cout.width(19);cout<<vec2[c].mean;
				}
				else{
					cout<<"mean"<<c+1<<"=";cout.fill(' ');cout.width(18);cout<<vec2[c].mean;
				}
			}
			cout<<endl;
			for (unsigned c = 0; c<vec2.size(); c+=1)
			{

				if(c<9){
					cout<<"stdev"<<c+1<<"=";cout.fill(' ');cout.width(18);cout<<vec2[c].stdev;
				}
				else{
					cout<<"stdev"<<c+1<<"=";cout.fill(' ');cout.width(17);cout<<vec2[c].stdev;
				}
			}
			break;
		case 6:
			cout<<endl;
			cout<<"输出完毕"<<endl;
			cout<<"该结果已输出到文件D:\\研究生\\统计学习方法\\3.读取文本\\result.txt"<<endl;
			break;
        default:
            printf("该层不存在, 请重新输入\n");
    }
} 
/**********************************
函数功能：打开文件
参数：ifs文件输入句柄 dir文件名
返回值：
**********************************/
bool Open_file(ifstream& ifs,string dir) {
	if (!ifs.is_open()){
		printf("打开文件%s失败,进行下一文件读取\n",dir.c_str());
		return false;
	}
	printf("打开文件%s成功\n",dir.c_str());
	return true;
}
/**********************************
函数功能：文件按行存入向量cls
参数：opt=0 加载偶数，=1加载奇数，ifs文件输入句柄 cls引用的向量
返回值：
**********************************/
void File2Cls(int opt, ifstream& ifs, vector<string> &cls)
{
	cls.clear();
	string temp;
	int flag=1;
	while (getline(ifs, temp)) 	
	{
		if(opt==1){				//读取奇数行到cls
			if(flag%2!=0)
				{cls.push_back(temp);flag ++;}
			else 
				flag++;
				continue;
			}
		else if(opt==0){		//读取偶数行到cls
			if(flag%2==0)
				{cls.push_back(temp);flag ++;}
			else 
				flag++;
				continue;
			}
	}
}
/**********************************
函数功能：以空格为界，找出cls每一列数据压入clm
参数：cls clm
返回值：clm
**********************************/
bool Cls2ClmAll(vector<string> &cls,vector<string> &clm)
{
	clm.clear();
	for (auto it = cls.begin(); it != cls.end(); it+=1)
	{
		istringstream is(*it);					   //用每一行的数据初始化一个字符串输入流；
		string s;
		while (is >> s) 						   //以空格为界，把istringstream中数据取出放入到依次s中
		{
				clm.push_back(s);
			
		}
	}
	return true;
}
/**********************************
函数功能：计算向量均值
参数：vec:需要计算的向量
返回值：均值
**********************************/
extern double Average(vector<double> &vec)
{
	double sum = accumulate(begin(vec),end(vec), 0.0);
	double mean =  sum / (vec.size()); 
	return mean;
}
/**********************************
函数功能：计算向量方差
参数：vec:需要计算的向量 mean:向量均值
返回值：方差
**********************************/
extern double Variance(vector<double> &vec,double mean)
{
	double accum  = 0.0;
	for_each (begin(vec), end(vec), [&](const double d) {
		accum  += (d-mean)*(d-mean);
	});
	double stdev = sqrt(accum/(vec.size()-1)); 
	return stdev;
};
/**********************************
函数功能：按33步长从clm中选数到block（block就是每一列），计算block的操作值
参数：clm block result
返回值：每一列的操作值按一行(均值 方差)存入result
**********************************/
bool Clm2BlockAndCalculation2Result(vector<string> &clm,vector<double> &block,vector<Data> &result)
{
	result.clear();
	for(int clm_num=0;clm_num<33;clm_num+=1)
	{
		block.clear();
		//printf("clm.size=%d\n",clm.size());
		for (auto it = clm.begin(); it != clm.end(); it=it+33)//行数
		{
			string x=*(it+clm_num);
			block.push_back(atof(x.c_str()));	
			//printf(" %f\n",atof(x.c_str()));//输出block（每列数据）
		}			
		Data data;
		data.mean=Average(block);
		data.stdev=Variance(block,data.mean);
		result.push_back(data);	
		//printf("第%d列的均值是%f,方差是%f\n",clm_num,data.mean,data.stdev);
	}
	return true;
}

double min(int x,int y){double z=(x<y)?x:y;return(z);}
/**********************************
函数功能：计算dst1-4最小值
参数：
返回值：dst1-4中的最小值
**********************************/
double minDistance()
{
	double z;
	z=min(dst1,dst2);
	z=min(z,dst3);
	z=min(z,dst4);
return z;
}
/**********************************
函数功能：根据文件名索引i，计算操均值方差存入对应的result1-4
参数：i文件标识，代表当前正在处理的文件
返回值：
**********************************/
void pushResult(int i,ofstream& out_file)
{
	switch (i)
	{
		case 1:
	 		Clm2BlockAndCalculation2Result(clm,block,result1);
			//输出横线
			Out_Info(i,4,"",kong1,kong2); 
			//输出每列均值与方差
			Out_Info(i,5,"",kong1,result1);
			break;
		case 2:
	 		Clm2BlockAndCalculation2Result(clm,block,result2);
			Out_Info(i,4,"",kong1,kong2); 
			Out_Info(i,5,"",kong1,result2);
			break;
		case 3:
	 		Clm2BlockAndCalculation2Result(clm,block,result3);
			Out_Info(i,4,"",kong1,kong2); 
			Out_Info(i,5,"",kong1,result3);
			break;
		case 4:
	 		Clm2BlockAndCalculation2Result(clm,block,result4);
			Out_Info(i,4,"",kong1,kong2); 
			Out_Info(i,5,"",kong1,result4);
			break;
	}
}
/**********************************
函数功能：以最小距离为准得出真实值，比较row_num行测试数据的真实值与期望值(偶数行第一列)是否相等，并根据真实值与期望值之间的关系生成混淆矩阵
参数：row_num文件行
返回值：
**********************************/
void Compare(int row_num)
{
	double z=minDistance();
	if(z==dst1)//按最小距离匹配真实值
	{	
		int real=1;
		int expect=atoi(clm[row_num+0].c_str());//real:真实值；expect：期望值(偶数行第一列)
		matrix[real-1][expect-1]+=1;
		printf("real=%d,except=%d\n",real,expect);
	}
	else if(z==dst2)
	{	
		int real=2;
		int expect=atoi(clm[row_num+0].c_str());//real:真实值；expect：期望值(偶数行第一列)
		matrix[real-1][expect-1]+=1;
		printf("real=%d,except=%d\n",real,expect);
	}
	else if(z==dst3)
	{	
		int real=3;
		int expect=atoi(clm[row_num+0].c_str());//real:真实值；expect：期望值(偶数行第一列)
		matrix[real-1][expect-1]+=1;
		printf("real=%d,except=%d\n",real,expect);
	}
	else if(z==dst4)			
	{	
		int real=4;
		int expect=atoi(clm[row_num+0].c_str());//real:真实值；expect：期望值(偶数行第一列)
		matrix[real-1][expect-1]+=1;
		printf("real=%d,except=%d\n",real,expect);
	}
}
/**********************************
函数功能：查找Chebyshev数组中的最大值
参数：
返回值：Chebyshev中的最大值
**********************************/
double maxChebyshev(double* Chebyshev)
{
	double max=Chebyshev[0];
	for(int i=0;i<31;i++)
	{
		if (max < Chebyshev[i])
		{
			max = Chebyshev[i];
		}
	}
	return max;
}

/**********************************
函数功能：计算row_num行的option距离
参数：row_num文件行 option距离种类:1-欧氏 2-曼哈顿 3-马氏
返回值：
**********************************/
void dstCalculation(int row_num,int option)
{
	//计算偶数行对每个文件均值的距离
	double count1=0.0,count2=0.0,count3=0.0,count4=0.0;
	for(unsigned clm_num=2;clm_num<33;clm_num++)
	{
		double temp=atof(clm[row_num+clm_num].c_str());
		//printf("现在处理的是class%d，第%d行，第%d列，值=%f\n",i,(row_num/33+1)*2,clm_num+1,temp);//这里(row_num/33+1)*2,clm_num+1是为了让输出的信息对应文件的格式而做的一种转换
		switch(option)
		{
		case 1://欧氏距离=sqrt((Xi-Xmean)^2)
		count1+=(temp-result1[clm_num].mean)*(temp-result1[clm_num].mean);
		count2+=(temp-result2[clm_num].mean)*(temp-result2[clm_num].mean);
		count3+=(temp-result3[clm_num].mean)*(temp-result3[clm_num].mean);
		count4+=(temp-result4[clm_num].mean)*(temp-result4[clm_num].mean);
		break;
		case 2://曼哈顿距离=abs(Xi-Xmean)
		count1+=abs(temp-result1[clm_num].mean);
		count2+=abs(temp-result2[clm_num].mean);
		count3+=abs(temp-result3[clm_num].mean);
		count4+=abs(temp-result4[clm_num].mean);
		break;
		case 3://标准欧氏距离=sqrt{[(Xi-Xmean)/sqrt(Xstdev)]^2}
		count1+=(temp-result1[clm_num].mean)/sqrt(result1[clm_num].stdev)*(temp-result1[clm_num].mean)/sqrt(result1[clm_num].stdev);
		count2+=(temp-result2[clm_num].mean)/sqrt(result2[clm_num].stdev)*(temp-result2[clm_num].mean)/sqrt(result2[clm_num].stdev);
		count3+=(temp-result3[clm_num].mean)/sqrt(result3[clm_num].stdev)*(temp-result3[clm_num].mean)/sqrt(result3[clm_num].stdev);
		count4+=(temp-result4[clm_num].mean)/sqrt(result4[clm_num].stdev)*(temp-result4[clm_num].mean)/sqrt(result4[clm_num].stdev);
		break;
		case 4://切比雪夫距离=max(|Xi-Xmean|)
		Chebyshev1[clm_num]=abs(temp-result1[clm_num].mean);
		Chebyshev2[clm_num]=abs(temp-result2[clm_num].mean);
		Chebyshev3[clm_num]=abs(temp-result3[clm_num].mean);
		Chebyshev4[clm_num]=abs(temp-result4[clm_num].mean);
		break;
		}
		
	}		
	if(option==1 || option==3)
	{
		dst1 = sqrt(count1); 
		dst2 = sqrt(count2); 
		dst3 = sqrt(count3); 
		dst4 = sqrt(count4); 
	}
	else if(option==2)
	{
		dst1 = count1; 
		dst2 = count2; 
		dst3 = count3; 
		dst4 = count4; 
	}
	else if(option==4)
	{
		dst1 = maxChebyshev(Chebyshev1); 
		dst2 = maxChebyshev(Chebyshev2); 
		dst3 = maxChebyshev(Chebyshev3); 
		dst4 = maxChebyshev(Chebyshev4); 
	}
	
	
}
/**********************************
函数功能：计算row_num行的option距离，求得最小距离，以最小距离为准得出真实值，比较row_num行测试数据的真实值与期望值(偶数行第一列)是否相等，并根据真实值与期望值之间的关系生成混淆矩阵
参数：row_num文件行 option距离种类:1-欧氏 2-曼哈顿 3-马氏
返回值：
**********************************/
void dstCalculationAndCompare(int i,int option)
{
	for(unsigned row_num=0;row_num<clm.size();row_num+=33)
	{
			
		dstCalculation(row_num,option);
		switch(option)
		{
		case 1:
			printf("class%d第%d行的欧氏距离dst1=%f,dst2=%f,dst3=%f,dst4=%f\n",i,(row_num/33+1)*2,dst1,dst2,dst3,dst4);
			break;
		case 2:
			printf("class%d第%d行的曼哈顿距离dst1=%f,dst2=%f,dst3=%f,dst4=%f\n",i,(row_num/33+1)*2,dst1,dst2,dst3,dst4);
			break;
		case 3:
			printf("class%d第%d行的标准欧氏距离dst1=%f,dst2=%f,dst3=%f,dst4=%f\n",i,(row_num/33+1)*2,dst1,dst2,dst3,dst4);
			break;
		case 4:
			printf("class%d第%d行的切比雪夫距离dst1=%f,dst2=%f,dst3=%f,dst4=%f\n",i,(row_num/33+1)*2,dst1,dst2,dst3,dst4);
			break;
		}
		printf("最小距离为%f\n",minDistance());
		Compare(row_num);
	}
}
/**********************************
函数功能：输出混淆矩阵，分类精度，Kappa系数
参数：row_num文件行 option距离种类:1-欧氏 2-曼哈顿 3-马氏
返回值：
**********************************/
void matrixCalculation()
{
	double n=0;//总和
	double diag=0;//对角线和
	double matrix0j=0,matrix1j=0,matrix2j=0,matrix3j=0;//第1234行和
	double matrixi0=0,matrixi1=0,matrixi2=0,matrixi3=0;//第1234列和
	printf("混淆矩阵如下：\n");
	for(int i=0;i<4;++i)
	{
		for(int j=0;j<4;++j)
		{
			cout.setf(ios::left); 
			cout.width(5); 
			cout<<matrix[i][j];

			n+=matrix[i][j];
			if(i==j){diag+=matrix[i][j];}
			if(i==0){matrix0j+=matrix[i][j];}
			if(i==1){matrix1j+=matrix[i][j];}
			if(i==2){matrix2j+=matrix[i][j];}
			if(i==3){matrix3j+=matrix[i][j];}
			if(j==0){matrixi0+=matrix[i][j];}
			if(j==1){matrixi1+=matrix[i][j];}
			if(j==2){matrixi2+=matrix[i][j];}
			if(j==3){matrixi3+=matrix[i][j];}
		}
		printf("\n");
	}
	double p0=diag/n;
	double pe=(matrix0j*matrixi0+matrix1j*matrixi1+matrix2j*matrixi2+matrix3j*matrixi3)/(n*n);
	double kappa=(p0-pe)/(1-pe);
	printf("分类精度p0=%f,pe=%f\n",p0,pe);
	printf("输出kappa=%f\n",kappa);
}

int main(){
	//此循环为计算class1-4奇数行的均值与方差，输出至result1-4
	for(int i=1;i<=4;i++)
	{

		
		/*获取文件名*/
		string dir=getFilename(i);
		ifstream file(dir);
		ofstream out_file(file_dir,std::ios_base::app);//追加方式

		/*获取时间*/
		string time=getTime();
		Out_Info(i,1,time,kong1,kong2);

		/*打开文件*/
		if(Open_file(file,dir)==false){continue;};

		/*加载训练集（文件按奇数行存入向量cls）,1为加载奇数行，0位加载偶数行*/
		File2Cls(1,file,cls);

		/*以空格为界按行压入clm*/
		Cls2ClmAll(cls,clm);
		
		/*clm格式化输出文本数据*/
		//Out_Info(i,3,out_file,"",clm,kong2); 
		
		/*根据文件名，计算操作值存入对应的result*/
		pushResult(i,out_file);
	}


	int option;//距离选项
	do
	{
		printf("\n请输入希望求得的距离：\n1:欧氏距离\n2:曼哈顿距离\n3:标准欧氏距离\n4:切比雪夫距离\n");
		cin>>option;
	}while(option!=1 && option!=2 && option!=3 && option!=4);//条件为假则跳出
	
	//此循环为计算class1-4每个偶数行到均值的距离
	for(int i=1;i<5;++i)
	{
		string dir=getFilename(i);
		ifstream file(dir);
		cout<<endl;
		if(Open_file(file,dir)==false){continue;};

		/*加载测试集，各文件偶数行到cls*/
		File2Cls(0,file,cls);

		/*以空格为界按行压入clm*/
		Cls2ClmAll(cls,clm);

		/*计算每一行的四个距离，并比较真实值与期望值以此赋值混淆矩阵*/
		dstCalculationAndCompare(i,option);
	}

	//计算并输出混淆矩阵，分类精度以及Kappa系数
	matrixCalculation();
	

system("pause");
return 0;
}




