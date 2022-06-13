///Huffman coding.
///I implemented it array based.

#include<iostream>
#include<ctime>
#include<iomanip>
#include<string>
#include<fstream>

using namespace std;



class Frequency_Table{

private:
    int ASCII;
    int frequency;


public:
    void set_ASCII(int x){
    ASCII=x;
    }


    void set_frequency(int y){
    frequency=y;
    }


    int get_ASCII(){
    return ASCII;
    }

    int get_frequency(){
    return frequency;
    }

};



class Code_Table{

private:
    int ASCII;
    string code;


public:
    void set_ASCII(int x){
    ASCII=x;
    }

    void set_code(string z){
     code=z;
    }

    void add_code(string y){
    code+=y;

    }


    int get_ASCII(){
    return ASCII;
    }

    string get_code(){
    return code;
    }

};




class Huffman_Array{

 private:

  string latter;
  int total_frequency;
  bool flag;



 public:

    void set_latter(char c){
    latter=c;

    }

   void add_latter(string a){
    latter+=a;

    }

  void set_total_frequency(int b){
    total_frequency+=b;
    }


    void set_flag(bool c){
    flag=c;
    }

    string get_latter(){
    return latter;
    }


    int get_total_frequency(){
    return total_frequency;
    }

    bool get_flag(){
    return flag;
    }

};


///Prototypes
int max(int A[128]);
int max(Code_Table *ct,int sz);
int num_dig(int n);
void huffman(int freq[128],int num_of_digit);
void populate_code_table(Code_Table *&ct,Huffman_Array *&hf,int sz);
int first_minimum(Huffman_Array *hf,int sz);
int second_minimum(Huffman_Array *hf,int sz,int first_min_pos);
void write_frequency_table_txt(int freq[128],int num_of_digit);
void radix_sort(Frequency_Table *&ft,int sz,int num_of_digit);
void counting_sort(Frequency_Table *&ft,int sz, int k);
int i_th_dig(int n, int k);
void counting_sort(Code_Table *&ct,int sz);
void match_ASCII(Code_Table *&ct,Huffman_Array *hf,int sz,int left,int right);
int bin_search(Code_Table *ct, int key, int l, int r);
void write_code_txt(Code_Table *ct,int sz);
void write_output_txt(Code_Table *ct,int sz);
int linear_search(Code_Table *ct,string a,int sz);
void write_code_table_txt(Code_Table *ct,int sz);
void read_code_table_txt();
void read_frequency_table_txt();
void read_output_txt();
void read_code_txt();
void read_input_txt();

int main(){

    ifstream infile;
	ofstream outfile;
    char ch;
	string a;
	outfile.open("input.txt");
	cout<<"Enter your txt: ";
	getline(cin,a);
	outfile<<a;
	outfile.close();

    int c;

        cout<<"Do to want to append any txt?"<<endl;
        cout<<"\t If Yes Enter [1] "<<endl;
        cout<<"\t If No  Enter [2]"<<endl;
        cout<<endl;
 do{
        cout<<"Your choice :";
        cin>>c;
        switch(c){
        case 1: outfile.open("input.txt",ios_base::app);
                cout<<"Enter your txt: ";
                cin.ignore();
                getline(cin,a);
                outfile<<endl;
                outfile<<a;
                outfile.close();
                break;
        }
   }while(c!=2);



	if(outfile)cout<<"Write in file successfully"<<endl;
	else cout<<"Failed!"<<endl;

    cout<<endl<<endl;

	int freq[128];
	for(int i=0;i<128;i++)freq[i]=0;
	infile.open("input.txt");

    while(infile.get(ch)){
        freq[(int)ch]++;
	}
	infile.close();
    cout<<endl;

    int num_of_digit=num_dig(max(freq));
    huffman(freq,num_of_digit);
    write_frequency_table_txt(freq,num_of_digit);

    read_input_txt();
    read_code_txt();
    read_output_txt();
    read_frequency_table_txt();
    read_code_table_txt();

    system("pause");
}



int max(int A[128]){
	int mx=A[0];
	for(int i=1;i<128;i++){
		if(A[i]>mx)mx=A[i];
	}
	return mx;
}

int max(Code_Table *ct,int sz){
	int mx=ct[0].get_code().length();
	for(int i=1;i<sz;i++){
		if(ct[i].get_code().length()>mx){
                mx=ct[i].get_code().length();
        }
	}
	return mx;
}


int num_dig(int n){
	if(n<10)return 1;
	return 1+num_dig(n/10);
}






void huffman(int freq[128],int num_of_digit){

int sz=0;
Frequency_Table *ft;

   for(int i=0;i<128;i++){
      if(freq[i]!=0){
      sz++;
    }
	}


ft=new Frequency_Table[sz];
for(int i=0;i<sz;i++){
 ft[i].set_ASCII(-1);
 ft[i].set_frequency(-1);
}


int j=0;
   for(int i=0;i<128;i++){
      if(freq[i]!=0){
      ft[j].set_ASCII(i);
      ft[j].set_frequency(freq[i]);
      j++;
    }
	}

radix_sort(ft,sz,num_of_digit);


Code_Table *ct=new Code_Table[sz];
for(int i=0;i<sz;i++){
    ct[i].set_ASCII(ft[i].get_ASCII());

}

 counting_sort(ct, sz);


Huffman_Array *hf=new Huffman_Array[sz];

for(int i=0;i<sz;i++){

    hf[i].set_latter((char)(ft[i].get_ASCII()));
    hf[i].set_total_frequency(ft[i].get_frequency());
    hf[i].set_flag(true);
}




 populate_code_table(ct,hf,sz);

 write_code_txt(ct,sz);
 write_output_txt(ct,sz);
 write_code_table_txt(ct,sz);

}




void populate_code_table(Code_Table *&ct,Huffman_Array *&hf,int sz){

while(true){
    int first_min_pos=first_minimum(hf,sz);
    int second_min_pos=second_minimum(hf,sz,first_min_pos);

    if(second_min_pos==-1){



    for(int i=0;i<sz;i++){
        string a="";
        string b=ct[i].get_code();

        for(int j=b.length()-1;j>=0;j--){
            a+=b[j];
        }

        ct[i].set_code(a);

    }


    break;

    }
    int left;
    int right;

    if(first_min_pos<second_min_pos){
        left=first_min_pos;
        right=second_min_pos;

    }

    else if(first_min_pos>second_min_pos){
        left=second_min_pos;
        right=first_min_pos;

    }

    match_ASCII(ct,hf,sz,left,right);

    hf[left].add_latter(hf[right].get_latter());
    hf[left].set_total_frequency(hf[right]. get_total_frequency());


    hf[right].set_flag(false);

}
}


int first_minimum(Huffman_Array *hf,int sz){
int first_min_pos=-1;
for(int i=0;i<sz;i++){
    if(hf[i].get_flag()==true && (first_min_pos==-1 || (hf[i].get_total_frequency()>=0 && hf[first_min_pos].get_total_frequency()>=0 && hf[i].get_total_frequency()<hf[first_min_pos].get_total_frequency()))){
    first_min_pos=i;
    }
}

return first_min_pos;
}


int second_minimum(Huffman_Array *hf,int sz,int first_min_pos){

int second_min_pos=-1;

for(int i=0;i<sz;i++){

    if(i==first_min_pos){
        continue;
    }
  else if(hf[i].get_flag()==true && (second_min_pos==-1 || (hf[i].get_total_frequency()>=0 && hf[second_min_pos].get_total_frequency()>=0 && hf[i].get_total_frequency()<hf[second_min_pos].get_total_frequency()))){
    second_min_pos=i;
    }
    }
    return second_min_pos;
}





void match_ASCII(Code_Table *&ct,Huffman_Array *hf,int sz,int left,int right){

string a=hf[left].get_latter();

for(int i=0;i<a.length();i++){
    int pos=bin_search(ct,(int)a[i],0,sz-1);
    ct[pos].add_code("0");
}


string b=hf[right].get_latter();
for(int i=0;i<b.length();i++){
    int pos=bin_search(ct,(int)b[i],0,sz-1);
    ct[pos].add_code("1");
}
}



///I'm doing binary search for "Code_Table" where I can search based on it's ASCII.
///Example: If i want to know A's huffman code (Assumed A is in our array) then I'll use it.

int bin_search(Code_Table *ct, int key, int l, int r){
	int mid=(l+r)/2;
	if(ct[mid].get_ASCII()==key)return mid;
	if(ct[mid].get_ASCII()>key)return bin_search(ct,key,l,mid-1);
	if(ct[mid].get_ASCII()<key)return bin_search(ct,key,mid+1,r);
}



///It use for decoding that means it'll search which code match with which latter.
///Then it'll give me that position.
///I call it from "void write_output_txt(Code_Table *ct,int sz)" function.
///Here I can't use binary search because i sorted my "Code_Table" type array on ASCII.
int linear_search(Code_Table *ct,string a,int sz){

for(int i=0;i<sz;i++){
    if(ct[i].get_code()==a){
        return i;
    }
}

return -1;
}





///Using counting sort for sorting Code_Table array on their ASCII.

void counting_sort(Code_Table *&ct,int sz){
	Code_Table *c=new Code_Table [sz];
	int *F=new int[128];
	for(int i=0;i<128;i++){
            F[i]=0;
	}

	for(int i=0;i<sz;i++) F[ct[i].get_ASCII()]++;




	for(int i=1;i<128;i++) F[i]=F[i]+F[i-1];


	for(int i=sz-1;i>=0;i--){
         F[ct[i].get_ASCII()]=F[ct[i].get_ASCII()]-1;
		c[F[ct[i].get_ASCII()]].set_ASCII(ct[i].get_ASCII());
		c[F[ct[i].get_ASCII()]].set_code(ct[i].get_code());

	}



	for(int i=0;i<sz;i++){
            ct[i].set_ASCII(c[i].get_ASCII());
            ct[i].set_code(c[i].get_code());
	}
}






///For Frequency Array. Sort them on their frequency. Because It's follow optimal marge property.


void radix_sort(Frequency_Table *&ft,int sz,int num_of_digit){

	for(int i=1;i<=num_of_digit;++i){
		counting_sort(ft,sz,i);
	}

}


void counting_sort(Frequency_Table *&ft,int sz, int k){
	Frequency_Table *f=new Frequency_Table [sz];
	int *F=new int[10];
	for(int i=0;i<10;i++){
            F[i]=0;
	}

	for(int i=0;i<sz;i++) F[i_th_dig(ft[i].get_frequency(),k)]++;




	for(int i=1;i<10;++i) F[i]=F[i]+F[i-1];


	for(int i=sz-1;i>=0;i--){
        F[i_th_dig(ft[i].get_frequency(),k)]=F[i_th_dig(ft[i].get_frequency(),k)]-1;
		f[F[i_th_dig(ft[i].get_frequency(),k)]].set_ASCII(ft[i].get_ASCII());
		f[F[i_th_dig(ft[i].get_frequency(),k)]].set_frequency(ft[i].get_frequency());

	}



	for(int i=0;i<sz;i++){
            ft[i].set_ASCII(f[i].get_ASCII());
            ft[i].set_frequency(f[i].get_frequency());
	}
}

int i_th_dig(int n, int k){
	for(int i=1;i<k;++i)n/=10;
	return n%10;
}





///File writing part.

 void write_code_txt(Code_Table *ct,int sz){
 ifstream infile;
 ofstream outfile;
 char ch;
 infile.open("input.txt");
 outfile.open("code.txt");

     while(infile.get(ch)){
        int pos=bin_search(ct, (int) ch, 0, sz-1);
        outfile<<ct[pos].get_code();

     }
    infile.close();
    outfile.close();

 }




 void write_output_txt(Code_Table *ct,int sz){
 ifstream infile;
 ofstream outfile;
 char ch;
 string a="";
 infile.open("code.txt");
 outfile.open("output.txt");
  while(infile.get(ch)){
        a+=ch;
        int pos=linear_search(ct,a,sz);
        if(pos!=-1){
            outfile<<(char)ct[pos].get_ASCII();
            a="";
        }
     }

infile.close();
outfile.close();

}







void write_code_table_txt(Code_Table *ct,int sz){

ofstream outfile;
int space_for_code=num_dig(max(ct,sz));

outfile.open("code_table.txt");
outfile<<left<<setw(10)<<"Letter" << right<<setw(6)<<"ASCII" << right<<setw(space_for_code+8)<<"Code"<<endl;
for(int i=0;i<sz;i++){
    if(ct[i].get_ASCII()==10){
        outfile<<left<<setw(10)<<"New Line"<< right<<setw(6)<<ct[i].get_ASCII()<< right<<setw(space_for_code+8)<<ct[i].get_code() <<endl;
    }
    else if(ct[i].get_ASCII()==32){
        outfile<<left<<setw(10)<<"Space"<< right<<setw(6)<<ct[i].get_ASCII()<< right<<setw(space_for_code+8)<<ct[i].get_code() <<endl;
    }
    else
          outfile<<left<<setw(10)<<(char)ct[i].get_ASCII()<< right<<setw(6)<<ct[i].get_ASCII()<< right<<setw(space_for_code+8)<<ct[i].get_code() <<endl;
}
outfile.close();

}




void  write_frequency_table_txt(int freq[128],int num_of_digit){

   ofstream outfile;

    int space_for_frequency=num_of_digit;

    if (space_for_frequency>=9){
        space_for_frequency=space_for_frequency+3;
    }
    else{
        space_for_frequency=12;
    }


    outfile.open("frequency_table.txt");
	outfile<<left<<setw(10)<<"Letter" << right<<setw(8)<<"ASCII" << right<<setw(space_for_frequency)<<"Frequency"<<endl;
    for(int i=0;i<128;i++){
      if(freq[i]!=0){
        if(i==10){
            outfile<<left<<setw(10)<<"New Line" << right<<setw(8)<<i << right<<setw(space_for_frequency)<<freq[i]<<endl;
        }
        else if(i==32){
            outfile<<left<<setw(10)<<"Space" << right<<setw(8)<<i << right<<setw(space_for_frequency)<<freq[i]<<endl;
        }
        else
        outfile<<left<<setw(10)<<(char)i << right<<setw(8)<<i << right<<setw(space_for_frequency)<<freq[i]<<endl;
    }
	}
	outfile.close();

}



///read part

void read_input_txt(){

  cout<<"input.txt :"<<endl;
  ifstream infile;
  char ch;

  infile.open("input.txt");
     while(infile.get(ch)){
		cout<<ch;
	}
	infile.close();

}


void read_code_txt(){
  cout<<endl<<endl;
  cout<<"code.txt :"<<endl;
  ifstream infile;
  char ch;

  infile.open("code.txt");
     while(infile.get(ch)){
		cout<<ch;
	}
	infile.close();

}


void read_output_txt(){
  cout<<endl<<endl;
  cout<<"output.txt :"<<endl;
  ifstream infile;
  char ch;

  infile.open("output.txt");
     while(infile.get(ch)){
		cout<<ch;
	}
	infile.close();

}

void read_frequency_table_txt(){
  cout<<endl<<endl;
  cout<<"frequency_table.txt :"<<endl<<endl;
  ifstream infile;
  char ch;

  infile.open("frequency_table.txt");
     while(infile.get(ch)){
		cout<<ch;
	}
	infile.close();
}


void read_code_table_txt(){
  cout<<endl<<endl;
  cout<<"code_table.txt :"<<endl<<endl;
  ifstream infile;
  char ch;

  infile.open("code_table.txt");
     while(infile.get(ch)){
		cout<<ch;
	}
	infile.close();
}







