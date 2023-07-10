#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
#include <cstdlib>

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

vector<string> split(string str, char del){
    string temp = "";
    vector<string> res;

      for(int i=0; i<(int)str.size(); i++){
        if(str[i] != del){
            temp += str[i];
        }
        else{
            res.pb(temp);
            temp = "";
        }
    }
    res.pb(temp);
    return res;
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
    int cnt = 0;

    while(!input.eof()){
        Data tmpData;
        vector<string> element;
        getline(input, line);
        element = split(line, ' ');

        tmpData.size = stoi(element[0]);
        tmpData.head = stoi(element[1]);
        tmpData.seektime = stoi(element[2]);
        
        for(int i = 3; i < element.size(); ++i)
            tmpData.data.pb(stoi(element[i]));
        
        fData.insert(pair<int, Data>(cnt, tmpData));
        ++cnt;
    }

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
    output.open(path);

    for(int i = 0; i < lines; ++i){
        string line = "";
        line += to_string(i+1) + ": \n";
        line += "FCFS: " + to_string(fcfs[i]) + ", Seek sequence: ";
        for(int x : fcfs_sequence[i]) line += "[" + to_string(x) + "] ";
        line += "\n";
        
        line += "Elevator: " + to_string(fcfs[i]) + ", Seek sequence: ";
        for(int x : elevator_sequence[i]) line += "[" + to_string(x) + "] ";
        line += "\n";

        output<<line;
    }

    output.close();
}

void dataRandomOutput(string path, Data oData){
    fstream output;
    output.open(path);

    string line = to_string(oData.size) + " " + to_string(oData.head) + " " + to_string(oData.seektime);
    for(int e : oData.data){
        line += " " + to_string(e);
    }
    line += "\n";
    output<<line;
    output.close();
}

int FCFS(int head, Data data, vector<int>& seek_sequence){
    int res = 0;

    cout << "\nData: "; data.display();

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

    cout << "\nData: "; data.display();

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
    
    fcfs = FCFS(uData.head, uData, seek_sequence);
    cout << "FCFS seek time: " << fcfs << endl;
    cout << "Seek sequence: "; sequence(seek_sequence);
    seek_sequence.clear();

    elevator = Elevator(uData.head, uData, seek_sequence);
    cout << "Elevator seek time: " << elevator << endl;
    cout << "Seek sequence: "; sequence(seek_sequence); 
    seek_sequence.clear();
}

void fCase(){
    map<int, Data> fData;
    vector<int> fcfs, elevator;
    vector<vector<int>> fcfs_se, elevator_se;


    fileInput("./input.txt", fData);
    
    for(int i = 0; i < lines; ++i){
        vector<int> fcfs_sequence, elevator_sequence;
        
        fcfs.pb(FCFS(fData[0].head, fData[0], fcfs_sequence));
        elevator.pb(Elevator(fData[0].head, fData[0], elevator_sequence));

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
    cout << "Data random: " << endl;
    cout << "Size: 1..20 Seektime: 1..10 Head and data inside: 1..100";
    cout << "Enter numbers of processes: ";
    cin >> n;

    for(int i = 0; i < n; ++i){
        Data rData;
        rData.size = rand() % s;
        rData.head = rand() % N;
        rData.seektime = rand() % s; 
        for(int j = 0; j < rData.size; ++j) rData.data.pb(rand() % N);

        dataRandomOutput("./input.txt", rData);
    }
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
            option--;
        }while( option > 3  || option < 0);
   
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
            default:
                exit;
        }

    }

    return 0;
}