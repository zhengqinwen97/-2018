#include "predict.h"
#include <stdio.h>

#include <iostream>
#include <map>
#include <string.h>
#include <vector>
#include <sstream>
using namespace std;


//你要完成的功能总入口

//map<string, int> numperday(char * data[MAX_DATA_NUM]);
//void print_map(map<string, int> mymap);

class Decomp{
	private:
		
	typedef struct Data{
		string vm;
		string date;
		int vm_num;
		int date_num;
		Data(char data[MAX_DATA_NUM]){
			string temp = data;//, sub_vm, sub_date;
			int begin = 0, count = 0;
			for(size_t j = 0; j < temp.size(); j++){
				if(temp[j] == '\t'){
					count++;
					if(count == 1)
						begin = j+1;
					if(count == 2){
						vm = temp.substr(begin, j-begin);
						begin = j+1;
					}
				}
				if(temp[j] == ' '){
					date = temp.substr(begin, j-begin);
					break;
				}
			}
			vm_num = de_vm(vm);
			date_num = de_date(date);
		}
	}*pData;





	public:
	static int de_vm(string myvm){
		int temp;
		string temp_sub = myvm.substr(6, myvm.size()-6);
		str2int(temp, temp_sub);
		return temp;
	}


	static int de_date(string mydate){
		int temp = 0;
		temp += de_year(mydate.substr(0, 4));
		temp += de_month(mydate.substr(5, 2));
		int temp_num;
		str2int(temp_num, mydate.substr(8, 2));
		temp += temp_num;
		return temp;
	}


	static void str2int(int &int_temp, string string_temp){
		stringstream stream(string_temp);
		stream>>int_temp;
	}
	
	static int de_month(string mon){
		int temp, ans = 0;
		int lemp[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
		str2int(temp, mon);
		for(int i = 0; i < temp-1; i++)
			ans += lemp[i];
		return ans;
	}

	static int de_year(string year){
		int temp_num = 0;
		int ans = 0;
		str2int(temp_num, year);
		ans += temp_num-2015;
		return ans * 365;
	}
	
	static void decomp_input(char * info[MAX_INFO_NUM], int& max_sever_cpu, int &max_sever_mem, int &max_sever_disk, int &my_vm_num, vector<vector<int>> &myvec, int &dura_time){
		
		string temp = info[0];
		int count = 0, begin;
		for(size_t i = 0; i < temp.size(); i++){
			if(count == 0 && temp[i] == ' '){
				str2int(max_sever_cpu, temp.substr(0, i));
				begin = i+1;
				count++;
				continue;
			}
			if(count == 1 && temp[i] == ' '){	
				str2int(max_sever_mem, temp.substr(begin, i));
				str2int(max_sever_disk, temp.substr(i+1, temp.size()));
				break;
			}
		}
		
		temp = info[2];
		str2int(my_vm_num, temp);

		count = 0; begin = 0;
		for(int i = 3; i < 3+my_vm_num; i++){
			temp = info[i]; 
			vector<int> temp_vec;
			for(size_t j = 0; j < temp.size(); j++){
				if(count == 0 && temp[j] == ' '){
					temp_vec.push_back(de_vm(temp.substr(begin, j) ));
					begin = j+1;
					count++;
					continue;
				}
				if(count == 1 && temp[j] == ' '){
					int a;
					str2int(a, temp.substr(begin, j));
					temp_vec.push_back(a);
					str2int(a, temp.substr(j+1, temp.size()));
					temp_vec.push_back(a);
					break;
				}
			}
			myvec.push_back(temp_vec);
			count = 0;
			begin = 0;
		}
		temp = info[6+my_vm_num];
		int start = de_date(temp.substr(0, 11));
		temp = info[7+my_vm_num];
		int end = de_date(temp.substr(0, 11));
		dura_time = end-start;
	}
	
	static void decomp_data(char* data[MAX_DATA_NUM], vector<vector<int>>& my_data){
		int start_date = Data(data[0]).date_num;
		int data_length;
		for(data_length = 0; data[data_length] != NULL; data_length++){}
		int end_date = Data(data[data_length-1]).date_num;
		int time_length = end_date - start_date + 1;
		
		vector<vector<int >> temp_data(25, vector<int>(time_length, 0));
		my_data = temp_data;
		
		for(int i = 0; i < data_length; i++){
			pData curr_data = new Data(data[i]);
			int curr_flavor = curr_data->vm_num;
			int curr_date = curr_data->date_num - start_date;
			my_data[curr_flavor][curr_date]++;
		}
	}

};


void predict_server(char * info[MAX_INFO_NUM], char * data[MAX_DATA_NUM], int data_num, char * filename)
{

	static vector<vector<int> > Flavor_Info;
	static int Max_Sever_Cpu;
	static int Max_Sever_Mem;
	static int Max_Sever_Disk;
	static int Flavor_Num;
	static int Dura_Time;
	static vector<vector<int>> My_Data;

	Decomp::decomp_input(info, Max_Sever_Cpu, Max_Sever_Mem, Max_Sever_Disk, Flavor_Num, Flavor_Info, Dura_Time);
	Decomp::decomp_data(data, My_Data);

	/*for(size_t i = 0; i < My_Data.size(); i++){
		for(sisize_tt j = 0; j < My_Data[i].size(); j++)
			cout << My_Data[i][j] << " ";
		cout << endl;
	}*/

	




	//需要输出的内容
	char * result_file = (char *)"17\n\n0 8 0 20";
	// 直接调用输出文件的方法输出到指定文件中(ps请注意格式的正确性，如果有解，第一行只有一个数据；第二行为空；第三行开始才是具体的数据，数据之间用一个空格分隔开)
	write_result(result_file, filename);
}





/*
map<string, int> numperday(char * data[MAX_DATA_NUM]){
	map<string, int> mymap;
	for(int i = 0; data[i] != NULL; i++){
		string temp = data[i];
		int count = 0, begin = 0;
		for(int j = 0; j < temp.size(); j++){
			if(temp[j] == '\t'){
				count++;
				if(count == 1)
					begin = j+1;
				if(count == 2){
					temp = temp.substr(begin, j-begin);
					break;
				}
			}
		}
		if(mymap.find(temp) == mymap.end())
			mymap[temp] = 1;
		else
			mymap[temp]++;
	}

	return mymap;
}


void print_map(map<string, int> mymap){
	for(map<string, int>::iterator it = mymap.begin(); it != mymap.end(); it++)
		cout << "key-->" << it->first << "  value-->" << it->second << endl;
}*/
