#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
#include <cstdlib>
#include <string>

using namespace std;

#define pb push_back

int lines;

map<int, vector<int>> fData;

class Data{
    public: 
        int size, seektime, head;
        vector<int> data;
        
        void display(){
            for(int e: data) cout << "[" << e << "]"; 
        };
};

//file data will be: [size] [head] [seektime] [data..]

void sequence(vector<int> a){
    for(int e: a) cout << e << " ";
}

int random(int &x, int max){
	x = rand() % max;
	while(x == 0) x = rand() % max;
	return x;
}

void userInput(Data &data){
    cout << "Input\n";

    cout << "Number of cylinders: ";
    cin >> data.size;

    cout << "Disk reading from: ";
    cin >> data.head;

    cout << "Seektime: ";
    cin >> data.seektime;

    cout << " Data: ";
    for(int i = 0; i < data.size; ++i) {
        int tmp;
        cin >> tmp;
        data.data.pb(tmp);
    }
}

void fileInput(string path, map<int, Data>& fData){
    ifstream input(path);
    
    string line;
    lines = 0;

    while(!input.eof()){
        Data tmpData;
        vector<string> element;
        
	input >> tmpData.size;
	input >> tmpData.head;
	input >> tmpData.seektime;

	for(int i = 0; i < tmpData.size; ++i){
		int tmp;
		input >> tmp;
		tmpData.data.pb(tmp);
	}
       fData.insert(pair<int, Data>(lines, tmpData));
        ++lines;
    }
	lines--;	
    input.close();
}

void fileOutput(string path, vector<int> fcfs, vector<int> elevator, vector<vector<int>> fcfs_sequence, vector<vector<int>> elevator_sequence){
    
    /*
        File ouput of 1 element/line :
        i:
        FCFS: res, Seek sequence: [..]
        Elevator: res, Seek sequence: [..]
    */
    
    fstream output;
    output.open(path, std::ofstream::out | std::ofstream::trunc);

    for(int i = 0; i < lines; ++i){
        string line = "";
        line += to_string(i+1) + ": \n";
        line += "FCFS: " + to_string(fcfs[i]) + ", Seek sequence: ";
        for(int x : fcfs_sequence[i]) line += "[" + to_string(x) + "] ";
        line += "\n";
        
        line += "Elevator: " + to_string(fcfs[i]) + ", Seek sequence: ";
        for(int x : elevator_sequence[i]) line += "[" + to_string(x) + "] ";
        output << line << endl;
    }

    output.close();
}

void dataRandomOutput(string path, vector<string> lines){
    fstream output;
    output.open(path, std::ofstream::out | std::ofstream::trunc);

    for(string line : lines) output<<line;

    output.close();
}

int FCFS(int head, Data data, vector<int>& seek_sequence){
    int res = 0;

    for(int i : data.data){
        seek_sequence.pb(i);
        res += abs(head - i);
        head = i;
    }

    return res *= data.seektime;
}

int Elevator(int head, Data data, vector<int>& seek_sequence){
    int res;
    int distance, cur_track;
    vector<int> left, right;


    for(int o : data.data){
        if(o > head) right.pb(o);
        if(o < head) left.pb(o);
    }

    sort(left.begin(), left.end());
    sort(right.begin(), right.end());

    for(int o : right){
        seek_sequence.pb(o);
        res += abs(head - o);
        head = o;
    }

    for(int o : left){
        seek_sequence.pb(o);
        res += abs(head - o);
        head = o;
    }

    return res *= data.seektime;
}

void cCase(){
    Data uData;
    vector<int> seek_sequence;
    int fcfs, elevator;

    userInput(uData);
    
    cout << "\nData: "; uData.display();
    fcfs = FCFS(uData.head, uData, seek_sequence);
    cout << "FCFS seek time: " << fcfs << endl;
    cout << "Seek sequence: "; sequence(seek_sequence);
    cout << endl;
    seek_sequence.clear();

    cout << "\nData: "; uData.display();
    elevator = Elevator(uData.head, uData, seek_sequence);
    cout << "Elevator seek time: " << elevator << endl;
    cout << "Seek sequence: "; sequence(seek_sequence); 
    cout << endl;
    seek_sequence.clear();
}

void fCase(){
    map<int, Data> fData;
    vector<int> fcfs, elevator;
    vector<vector<int>> fcfs_se, elevator_se;


    fileInput("./input.txt", fData);
    
    for(int i = 0; i < lines; ++i){
        vector<int> fcfs_sequence, elevator_sequence;
        
        fcfs.pb(FCFS(fData[i].head, fData[i], fcfs_sequence));
        elevator.pb(Elevator(fData[i].head, fData[i], elevator_sequence));

        fcfs_se.pb(fcfs_sequence);
        elevator_se.pb(elevator_sequence);
    }
    fileOutput("./output.txt", fcfs, elevator, fcfs_se, elevator_se);
}

void rCase(){
    int N = 100;
    int s = 20;
    int ss = 10;
    int n;
    vector<string> lines;
    cout << "Data random: " << endl;
    cout << "Size: [1..20] | Seektime: [1..10] | Head and data inside: [1..100]" << endl;;
    cout << "Enter numbers of processes: ";
    cin >> n;

    for(int i = 0; i < n; ++i){
        Data rData;
	int tmp;

	random(rData.size, s);
	random(rData.head, N);
	random(rData.seektime, s);	
 
        for(int j = 0; j < rData.size; ++j) rData.data.pb(random(tmp, N));

	string line = to_string(rData.size) + " " + to_string(rData.head) + " " + to_string(rData.seektime);
   	for(int e : rData.data){
       		 line += " " + to_string(e);
    	}
    	line += "\n";
 
	lines.pb(line);
    }

    dataRandomOutput("./input.txt", lines);
}

int main(){
    bool running = true;
    int option;

    while(running){
        cout << "Choose options: " << endl;
        cout << "   1) File Input (input.txt)" << endl;
        cout << "   2) Custom Input" << endl;
        cout << "   3) Generate random cases (after generate please choose file input options)" << endl;
        cout << "   4) Exit" << endl;        
   
        do{
            cout << "Choose your option (1..3): ";
            cin >> option;
            option -= 1;
        } while( option > 3 || option < 0);
   
        switch(option){
            case 0:
                fCase();
                break;
            case 1:
                cCase();
                break;
            case 2:
                rCase();
                break;
	    case 3:
		running = false;
		break;
            default:
		running = false;
                break;
        }

    }

    return 0;
}
