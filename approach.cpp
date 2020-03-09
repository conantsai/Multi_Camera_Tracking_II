#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

using namespace std;

void getEcost(int **graph, vector <int> &nodes, vector <int> &nodes_datetime, int **ecost){
    int count = nodes.size()/2;
    int interval_info[count];

    for(int i=0; i<count; i=i+2){
        for(int j=0; j<count; j=j+2){
            if(graph[i][j] == 1){
                int lea = nodes_datetime[j+1];
                int arr = nodes_datetime[j+2];
                int interval = (arr - lea);
                cout << interval << endl;
                interval_info[j] = interval;
            }
        }
        break;
    }
        
    // for(int j=0; j<count; j++){
    //     cout << interval_info[j]<< endl;
    // }

}

void constructConflict(vector <int> &nodes, int **conflict){
    int count = nodes.size()/2;
    
    for(int i=0; i<count; i=i+2){
        for(int j=i+1; j<count; j=j+2){
            if ((nodes[i] == nodes[j]) || (nodes[i+1] == nodes[j+1]) || (nodes[i] == nodes[j+1]) || (nodes[i+1] == nodes[j])){
                conflict[i][j] = 1;
                conflict[j][i] = 1;
            }
        }
    }
}

int isGroup(int **group, int tracklet1, int tracklet2){
    if (tracklet1 > 9){
        tracklet1 -= 9;
    }
    if (tracklet2 > 9){
        tracklet2 -= 9;
    }

    if(group[tracklet1][tracklet2] == 1){
        return 1;
    }else
    {
        return 0;
    }
}

void constructGraph(vector <int> &nodes, int **cam1Groups, int **cam2Groups, int **graph){
    int count = nodes.size()/2;

     for(int i=0; i<count; i++){
        for(int j=0; j<count; j++){
            graph[i][j] = 0;
        }
    }

    for(int i=0; i<count; i=i+2){
        for(int j=i; j<count; j=j+2){
            if ((nodes[i] != nodes[j]) && (nodes[i+1] != nodes[j+1]) && (nodes[i] != nodes[j+1]) && (nodes[i+1] != nodes[j])
                 && (isGroup(cam1Groups, nodes[i], nodes[j]) || isGroup(cam2Groups, nodes[i+1], nodes[j+1]))){
                graph[i][j] = 1;
                graph[j][i] = 1;
            }
        }
    }
}

void createGroups(vector <string> &tracklet, int timeInterval,  int **group){
    int tracklet_size = tracklet.size()/3;

    for(int i=0; i<tracklet.size(); i=i+3){
        int Lea = stoi(tracklet[i+2]);
        group[i/3][i/3] = 0;
        for(int j=i+3; j<tracklet.size(); j=j+3){
            int Arr = stoi(tracklet[j+1]);
            if ((Arr + timeInterval*100) < Lea){
                group[i/3][j/3] = 1;
                group[j/3][i/3] = 1;
            }else{
                group[i/3][j/3] = 0;
                group[j/3][i/3] = 0;
            }
        }
    }
}

void createNode(vector <int> &nodes, vector <int> &nodes_datetime, int **cam1Groups, int **cam2Groups){
    vector <string> cam1_data;
    vector <string> cam2_data;

    fstream file;
    char content[100];

    file.open("data/cam1/tracker.txt", ios::in);
    if(!file){
        cout << "not founf file." << endl;
    }else{
        while(!file.eof()){
            file.getline(content, sizeof(content));

            vector <string> cam_info;
       
            char* chars_array = strtok(content, ":");
            while(chars_array){
                cam_info.push_back(chars_array);
                chars_array = strtok(NULL, ",");
            }
            
            for(int i=1; i<3; i++){
                vector <string> time_info;

                char *time = new char[cam_info[i].length() + 1];
                strcpy(time, cam_info[i].c_str());

                chars_array = strtok(time, "'");
                while(chars_array){
                    time_info.push_back(chars_array);
                    chars_array = strtok(NULL, "'");
                }

                cam_info[i] = to_string(stoi(time_info[0]) * 6000 + stoi(time_info[1]) * 100 + stoi(time_info[2]));
                delete [] time;
            }
            
            cam1_data.insert(cam1_data.end(), cam_info.begin(), cam_info.end());
        }
        file.close();
    }

    file.open("data/cam2/tracker.txt", ios::in);
    if(!file){
        cout << "not founf file." << endl;
    }else{
        while(!file.eof()){
            file.getline(content, sizeof(content));

            vector <string> cam_info;
       
            char* chars_array = strtok(content, ":");
            while(chars_array){
                cam_info.push_back(chars_array);
                chars_array = strtok(NULL, ",");
            }
            
            for(int i=1; i<3; i++){
                vector <string> time_info;

                char *time = new char[cam_info[i].length() + 1];
                strcpy(time, cam_info[i].c_str());

                chars_array = strtok(time, "'");
                while(chars_array){
                    time_info.push_back(chars_array);
                    chars_array = strtok(NULL, "'");
                }

                cam_info[i] = to_string(stoi(time_info[0]) * 6000 + stoi(time_info[1]) * 100 + stoi(time_info[2]));
                delete [] time;
            }
            
            cam2_data.insert(cam2_data.end(), cam_info.begin(), cam_info.end());
        }
        file.close();
    }

    for(int i=0; i<cam1_data.size(); i=i+3){
        for(int j=0; j<cam2_data.size(); j=j+3){
            if((cam1_data[i+1] < cam2_data[j+1]) && (cam1_data[i+2] < cam2_data[j+2])){
                if ((stoi(cam1_data[i+2]) + 6000) > stoi(cam2_data[i+1])){
                    nodes.push_back(stoi(cam1_data[i]));
                    nodes.push_back(stoi(cam2_data[j]));

                    nodes_datetime.push_back(stoi(cam1_data[i+1]));
                    nodes_datetime.push_back(stoi(cam1_data[i+2]));
                    nodes_datetime.push_back(stoi(cam2_data[j+1]));
                    nodes_datetime.push_back(stoi(cam2_data[j+2]));
                }
            }
        }
    }
    
    createGroups(cam1_data, 6, cam1Groups);
    createGroups(cam2_data, 6, cam2Groups);

    return ;
}


int main(void){
    vector <int> nodes;
    vector <int> nodes_datetime;

    int ** cam1Groups;
    cam1Groups = new int * [10];     
    for (int i=0; i<10; i++){   
        cam1Groups[i] = new int [10];
    }
    int ** cam2Groups;
    cam2Groups = new int * [10];     
    for (int i=0; i<10; i++){   
        cam2Groups[i] = new int [10];
    }

    createNode(nodes, nodes_datetime, cam1Groups, cam2Groups);

    int count = nodes.size()/2;
    int ** graph;
    graph = new int * [count];     
    for (int i=0; i<count; i++){   
        graph[i] = new int [count];
    }

    constructGraph(nodes, cam1Groups, cam2Groups, graph);

    int ** conflict;
    conflict = new int * [count];     
    for (int i=0; i<count; i++){   
        conflict[i] = new int [count];
    }

    constructConflict(nodes, conflict);

    int ** ecost;
    ecost = new int * [count];     
    for (int i=0; i<count; i++){   
        ecost[i] = new int [count];
    }

    getEcost(graph, nodes, nodes_datetime, ecost);
}
