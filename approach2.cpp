#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <cmath>
#include <random>
#include <sys/stat.h>
#include <dirent.h>
#include <algorithm>
// #include <opencv2/opencv.hpp>

using namespace std;
// using namespace cv;

void sortArr(float arr[], int n, vector<float> &vcost, vector<int> &vcost_list) 
{ 
    vector <pair<float, int>> vp; 

    for(int i=0; i<n; ++i) { 
        vp.push_back(make_pair(arr[i], i)); 
    } 

    sort(vp.begin(), vp.end()); 

    for (int i=0; i<vp.size(); i++){
        vcost[i] = vp[i].first;
        vcost_list.push_back(vp[i].second);
    }
} 

/*List all images's name in path.*/
void ListImages(string const &path, vector<string> &images) {
    images.clear();
    struct dirent *entry;

    /*Check if path is a valid directory path. */
    struct stat s;
    lstat(path.c_str(), &s);
    if (!S_ISDIR(s.st_mode)) {
      fprintf(stderr, "Error: %s is not a valid directory!\n", path.c_str());
      exit(1);
    }

    DIR *dir = opendir(path.c_str());
    if (dir == nullptr) {
      fprintf(stderr, "Error: Open %s path failed.\n", path.c_str());
      exit(1);
    }

    while ((entry = readdir(dir)) != nullptr) {
      if (entry->d_type == DT_REG || entry->d_type == DT_UNKNOWN) {
        string name = entry->d_name;
        string ext = name.substr(name.find_last_of(".") + 1);
        if ((ext == "JPEG") || (ext == "jpeg") || (ext == "JPG") || (ext == "jpg") ||
            (ext == "bmp") || (ext == "PNG") || (ext == "png")) {
          images.push_back(name);
        }
      }
    }
    closedir(dir);
}

void getVcost(vector <int> &nodes, vector <float> &vcost, int **vlist){
    int count = nodes.size()/2;

    for(int i=0; i<count; i++){
        vcost.push_back(0);
    }

    string cam1ImagePath = "./data/cam1";
    string cam2ImagePath = "./data/cam1";
    vector<string> cam1Images, cam2Images;

    ListImages(cam1ImagePath, cam1Images);
    ListImages(cam2ImagePath, cam2Images);

    for (auto &image_name : cam1Images) {
        cout << "\nLoad image : " << image_name << endl;
        // Mat image = imread(baseImagePath + image_name);
        
    }

    float tentative_vcost[] = {0.14672285, 0.14381562, 0.1353567,  0.15588763, 0.1463403,  0.16863456,
                               0.12309646, 0.1492267,  0.16153237, 0.19058863, 0.19089417, 0.11251639,
                               0.17780377, 0.1390506,  0.17028903, 0.1982498,  0.16413768, 0.22393119,
                               0.17677382, 0.24722998, 0.1929111,  0.22624995, 0.17198621, 0.23883565,
                               0.1960639,  0.2052911,  0.19811215, 0.16283216, 0.19804346, 0.2930768,
                               0.16826582, 0.19232088, 0.16800533, 0.17103117, 0.15392985, 0.16536156,
                               0.19001316, 0.18959997, 0.14417996, 0.25718294, 0.12243394, 0.1684318,
                               0.11499672, 0.17252813, 0.11693792, 0.12893153, 0.11575986, 0.1038857,
                               0.13509445, 0.16153058, 0.12470854, 0.15559669, 0.11962353, 0.16137322,
                               0.11978564, 0.14310523, 0.13311321, 0.13796748, 0.13607022, 0.21747227,
                               0.11271337, 0.14652318, 0.09649291, 0.12892737, 0.08773395, 0.13880596,
                               0.08227954, 0.13716899, 0.12763583, 0.17191024, 0.11489494, 0.18343618,
                               0.12447174, 0.16594496, 0.12711261, 0.14744289, 0.11944002, 0.15913079,
                               0.15994335, 0.20145054, 0.15634393, 0.15267796, 0.10948647, 0.14226468,
                               0.11392029, 0.17561714, 0.10140082, 0.13587547, 0.13907204, 0.23905422,
                               0.1617939,  0.14714501, 0.17126164, 0.16425205, 0.17148408, 0.1435445,
                               0.18630033, 0.17106136, 0.16114138, 0.25820304};

    int tentative_vnode[100][3] = {{0, 0, 10}, {1, 0, 11}, {2, 0, 12}, {3, 0, 13}, {4, 0, 14}, {5, 0, 15}, 
                          {6, 0, 16}, {7, 0, 17}, {8, 0, 18}, {9, 0, 19}, {10, 1, 10}, {11, 1, 11}, 
                          {12, 1, 12}, {13, 1, 13}, {14, 1, 14}, {15, 1, 15}, {16, 1, 16}, {17, 1, 17}, 
                          {18, 1, 18}, {19, 1, 19}, {20, 2, 10}, {21, 2, 11}, {22, 2, 12}, {23, 2, 13}, 
                          {24, 2, 14}, {25, 2, 15}, {26, 2, 16}, {27, 2, 17}, {28, 2, 18}, {29, 2, 19}, 
                          {30, 3, 10}, {31, 3, 11}, {32, 3, 12}, {33, 3, 13}, {34, 3, 14}, {35, 3, 15}, 
                          {36, 3, 16}, {37, 3, 17}, {38, 3, 18}, {39, 3, 19}, {40, 4, 10}, {41, 4, 11}, 
                          {42, 4, 12}, {43, 4, 13}, {44, 4, 14}, {45, 4, 15}, {46, 4, 16}, {47, 4, 17}, 
                          {48, 4, 18}, {49, 4, 19}, {50, 5, 10}, {51, 5, 11}, {52, 5, 12}, {53, 5, 13}, 
                          {54, 5, 14}, {55, 5, 15}, {56, 5, 16}, {57, 5, 17}, {58, 5, 18}, {59, 5, 19}, 
                          {60, 6, 10}, {61, 6, 11}, {62, 6, 12}, {63, 6, 13}, {64, 6, 14}, {65, 6, 15}, 
                          {66, 6, 16}, {67, 6, 17}, {68, 6, 18}, {69, 6, 19}, {70, 7, 10}, {71, 7, 11}, 
                          {72, 7, 12}, {73, 7, 13}, {74, 7, 14}, {75, 7, 15}, {76, 7, 16}, {77, 7, 17}, 
                          {78, 7, 18}, {79, 7, 19}, {80, 8, 10}, {81, 8, 11}, {82, 8, 12}, {83, 8, 13}, 
                          {84, 8, 14}, {85, 8, 15}, {86, 8, 16}, {87, 8, 17}, {88, 8, 18}, {89, 8, 19}, 
                          {90, 9, 10}, {91, 9, 11}, {92, 9, 12}, {93, 9, 13}, {94, 9, 14}, {95, 9, 15}, 
                          {96, 9, 16}, {97, 9, 17}, {98, 9, 18}, {99, 9, 19}};
    // vcost.assign(vcost1, vcost1+100);
    // sort(vcost.begin(), vcost.end()); 
    // sort(tentative_vcost, tentative_vcost+100); 

    vector <int> vcost_list;
    int vcost_n = sizeof(tentative_vcost) / sizeof(tentative_vcost[0]); 
    sortArr(tentative_vcost, vcost_n, vcost, vcost_list); 
                       
    for(int i=0; i<count; i++){
        for(int j=0; j<count; j++){
            vlist[i][j] = tentative_vnode[vcost_list[i]][j];
        }
    }
}

void getEcost(int **graph, vector <int> &nodes, vector <int> &nodes_datetime, float **ecost){
    int count = nodes.size()/2;
    int interval_info[count];

    for(int i=0; i<count; i++){
        for(int j=0; j<count; j++){
            if(graph[i][j] == 1){
                int lea = nodes_datetime[4*j+1];
                int arr = nodes_datetime[4*j+2];
                int interval = (arr - lea);
                interval_info[j] = interval;
            }else{
                interval_info[j] = 0;
            }
        }
        break;
    }

    float interval_sum = 0;
    for(int i=0; i<count; i++){
        interval_sum += interval_info[i];
    }
    float interval_mean = interval_sum/count;

    float interval_std = 0;
    for(int i=0; i<count; i++){
        interval_std += pow(interval_info[i]-interval_mean, 2);
    }
    interval_std = sqrt(interval_std/count);

    for(int i=0; i<count; i++){
        for(int j=0; j<count; j++){
            if(graph[i][j] == 1){
                int lea = nodes_datetime[4*j+1];
                int arr = nodes_datetime[4*j+2];
                int interval = (arr - lea);

                static const float inv_sqrt_2pi = 0.3989422804014327;
                float a = (interval - interval_mean) / interval_std;
                ecost[i][j] = (inv_sqrt_2pi / interval_std * std::exp(-0.5f * a * a)*100);
            }else{
                ecost[i][j] = 0;
            }
        }
    }

}

void constructConflict(vector <int> &nodes, int **conflict){
    int count = nodes.size()/2;
    
    for(int i=0; i<count; i++){
        for(int j=i+1; j<count; j++){
            if ((nodes[2*i] == nodes[2*j]) || (nodes[2*i+1] == nodes[2*j+1]) || (nodes[2*i] == nodes[2*j+1]) || (nodes[2*i+1] == nodes[2*j])){
                conflict[i][j] = 1;
                conflict[j][i] = 1;
            }
        }
    }
}

int isGroup(int **group, int tracklet1, int tracklet2){
    if (tracklet1 > 9){
        tracklet1 -= 10;
    }
    if (tracklet2 > 9){
        tracklet2 -= 10;
    }

    if(group[tracklet1][tracklet2] == 1){
        return true;
    }else{
        return false;
    }
}

void constructGraph(vector <int> &nodes, int **cam1Groups, int **cam2Groups, int **graph){
    int count = nodes.size()/2;

    for(int i=0; i<count; i++){
        for(int j=0; j<count; j++){
            graph[i][j] = 0;
        }
    }

    for(int i=0; i<count; i++){
        for(int j=i; j<count; j++){
            bool cam1_isGroup = isGroup(cam1Groups, nodes[2*i], nodes[2*j]);
            bool cam2_isGroup = isGroup(cam2Groups, nodes[2*i+1], nodes[2*j+1]);
            if ((nodes[2*i] != nodes[2*j]) && (nodes[2*i+1] != nodes[2*j+1]) && (nodes[2*i] != nodes[2*j+1]) && (nodes[2*i+1] != nodes[2*j]) && (cam1_isGroup || cam2_isGroup)){
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

void* new2d(int h, int w, int size)
{
    int i;
    void **p;

    p = (void**)new char[h*sizeof(void*) + h*w*size];
    for(i = 0; i < h; i++)
        p[i] = ((char *)(p + h)) + i*w*size;

    return p;
}

void* new2d_int(int h, int w, int size)
{
    int i;
    void **p;

    p = (void**)new int[h*sizeof(void*) + h*w*size];
    for(i = 0; i < h; i++)
        p[i] = ((int *)(p + h)) + i*w*size;

    return p;
}

void* new2d_float(int h, int w, int size)
{
    int i;
    void **p;

    p = (void**)new float[h*sizeof(void*) + h*w*size];
    for(i = 0; i < h; i++)
        p[i] = ((float *)(p + h)) + i*w*size;

    return p;
}

int main(void){
    vector <int> nodes;
    vector <int> nodes_datetime;

    int **cam1Groups = (int **)new2d_int(10, 10, 10);
    int **cam2Groups = (int **)new2d_int(10, 10, 10);
    // int ** cam1Groups;
    // cam1Groups = new int * [10];     
    // for (int i=0; i<10; i++){   
    //     cam1Groups[i] = new int [10];
    // }
    createNode(nodes, nodes_datetime, cam1Groups, cam2Groups);

    int count = nodes.size()/2;
    int **graph = (int **)new2d_int(count, count, count);
    constructGraph(nodes, cam1Groups, cam2Groups, graph);

    int **conflict = (int **)new2d(count, count, count);
    constructConflict(nodes, conflict);

    float **ecost = (float **)new2d_float(count, count, count);
    getEcost(graph, nodes, nodes_datetime, ecost);

    vector <float> vcost;
    // vector <int> vlist;
    int **vlist = (int **)new2d_int(count, 3, 10);

    getVcost(nodes, vcost, vlist);

    int maxLabel = 0;
    int maxCost = 1000;

    vector <float> nodeUse;
    vector <float> edgeUse;

    for(int i=0; i<count; i++){
        int label[100];
        for(int j=0; j<count; j++){
            label[j]=1;
        }
        // float **nodeTemp = (float **)new2d_float(count, 3, 10);
        vector <float> nodeTemp;
        // float **edgeTemp = (float **)new2d_float(count, 5, 10);
        vector <float> edgeTemp;

        label[i] = 1;
        int node_index = 0;
        int edge_index = 0;

        // nodeTemp[node_index][0] = vlist[i][1];
        // nodeTemp[node_index][1] = vlist[i][2];
        // nodeTemp[node_index][2] = vcost[i];
        // node_index ++;
        nodeTemp.push_back(vlist[i][1]);
        nodeTemp.push_back(vlist[i][2]);
        nodeTemp.push_back(vcost[i]);

        for(int j=0; j<count; j++){
            if(conflict[vlist[i][0]][vlist[j][0]] == 1){
                label[j] = 0;
            }
        }

        for(int j=0; j<count; j++){
            if(label[j] == 1 && j != i){
                if(graph[vlist[i][0]][vlist[j][0]] == 1){
                    nodeTemp.push_back(vlist[j][1]);
                    nodeTemp.push_back(vlist[j][2]);
                    nodeTemp.push_back(vcost[j]);


                    edgeTemp.push_back(vlist[i][1]);
                    edgeTemp.push_back(vlist[i][2]);
                    edgeTemp.push_back(vlist[j][1]);
                    edgeTemp.push_back(vlist[j][2]);
                    edgeTemp.push_back(ecost[vlist[i][0]][vlist[j][0]]);
                
                    for(int k=0; k<count; k++){
                        if(conflict[vlist[j][0]][vlist[k][0]]){
                            label[k] = 0;
                        }
                    }
                    
                }
            }
        }
        
        // int labels = nodeTemp.size()/3;
        // if(labels > maxLabel){
        //     maxLabel = labels;
        //     float nodeCost = 0;
        //     float edgeCost = 0;

        //     for(int j=0; j<nodeTemp.size()/3; j=j+3){
        //         nodeCost += nodeTemp[j+2];

        //     }
        //     for(int j=0; j<edgeTemp.size()/5; j=j+5){
        //         edgeCost += nodeTemp[j+4];
        //     }

        //     float cost = nodeCost; 

        //     if(cost <= maxCost){
        //         maxCost = cost;
        //         nodeUse = nodeTemp;
        //         edgeUse = edgeTemp;
        //         break;
        //     }    
        // }else if(labels == maxLabel){
        //     float nodeCost = 0;
        //     float edgeCost = 0;

        //     for(int j=0; j<nodeTemp.size()/3; j=j+3){
        //         nodeCost += nodeTemp[j+2];

        //     }
        //     for(int j=0; j<edgeTemp.size()/5; j=j+5){
        //         edgeCost += nodeTemp[j+4];
        //     }

        //     float cost = nodeCost; 

        //     if(cost <= maxCost){
        //         maxCost = cost;
        //         nodeUse = nodeTemp;
        //         edgeUse = edgeTemp;
        //         break;
        //     }    
        // }
    }

    // for(int i=0; i<nodeUse.size(); i=i+3){
    //     cout<<nodeUse[i]<<"<->"<<nodeUse[i+1]<<endl;
    // }

    // delete [] cam1Groups;
    // delete [] cam2Groups;
    // delete [] graph;
    // delete [] conflict;
    // delete [] ecost;
    // delete [] vlist;
}
