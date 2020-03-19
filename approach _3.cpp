#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <cmath>
#include <random>
#include <sys/stat.h>
#include <dirent.h>
#include <algorithm>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


// Computes the distance between two std::vectors
template <typename T>
double	vectors_distance(const vector<T>& a, const vector<T>& b){
	vector<double>	auxiliary;

	transform (a.begin(), a.end(), b.begin(), back_inserter(auxiliary),//
	[](T element1, T element2) {return pow((element1-element2),2);});
	auxiliary.shrink_to_fit();

	return  sqrt(std::accumulate(auxiliary.begin(), auxiliary.end(), 0.0));
} // end template vectors_distance

void sortArr(vector<float> &vcost_tmp, int n, vector<float> &vcost, vector<int> &vcost_list){ 
    vector <pair<float, int>> vp; 

    for(int i=0; i<n; ++i) { 
        vp.push_back(make_pair(vcost_tmp[i], i));
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
    vector <float> vcost_tmp;

    for(int i=0; i<count; i++){
        vcost_tmp.push_back(0);
    }
    for(int i=0; i<count; i++){
        vcost.push_back(0);
    }

    string cam1ImagePath = "./data/cam1/cam1_person";
    string cam2ImagePath = "./data/cam2/cam2_person";
    vector <string> cam1Images, cam2Images;
    vector <int> vnode;
    int index = 0;

    for(int i=0; i<nodes.size(); i=i+2){
        string cam1Path = "data/cam1/cam1_person" + to_string(nodes[i]) + ".png";
        string cam2Path = "data/cam2/cam2_person" + to_string(nodes[i+1]) + ".png";

        Mat cam1Image = imread(cam1Path);
        Mat cam2Image = imread(cam2Path);

        float cam1_feat[1][4][2][4] = {{{{0.1820118,  0.01131729, 0.08673228, 0.04008939},
                                        {0.09348882, 0.01959437, 0.04942593, 0.04631482}},
                                       {{0.08874577, 0.0175822,  0.04482096, 0.04295532},
                                        {0.09784839, 0.01864509, 0.04760801, 0.04495515}},
                                       {{0.14304025, 0.0206972,  0.07466555, 0.043155  },
                                        {0.1575067,  0.01871352, 0.08274077, 0.04338045}},
                                       {{0.20683368, 0.01561823, 0.09829947, 0.03691985},
                                        {0.17649174, 0.01418823, 0.08741158, 0.04082694}}}};

        float cam2_feat[1][4][2][4] = {{{{0.15523714, 0.01739082, 0.07491373, 0.04018689},
                                         {0.1804601,  0.01713482, 0.08733648, 0.03940007}},
                                        {{0.13846312, 0.01735787, 0.06631881, 0.04152053},
                                         {0.17667529, 0.01680345, 0.0869156,  0.04085378}},
                                        {{0.14191046, 0.01744963, 0.07034914, 0.03946224},
                                         {0.17104968, 0.01698326, 0.0827354,  0.03963856}},
                                        {{0.17638955, 0.01612297, 0.08563288, 0.03762322},
                                         {0.1606842,  0.01554271, 0.07915629, 0.04017135}}}};

        vector<float> cam1_feat_reshape;
        for(int m=0; m<1; m++){
            for(int n=0; n<4; n++){
                for(int k=0; k<2; k++){
                    for(int l=0; l<4; l++){
                        if(cam1_feat[m][n][k][l] < 0){
                            cam1_feat_reshape.push_back(0);
                        }else{
                            cam1_feat_reshape.push_back(cam1_feat[m][n][k][l]);
                        }
                    }
                }
            }
        }

        vector<float> cam2_feat_reshape;
        for(int m=0; m<1; m++){
            for(int n=0; n<4; n++){
                for(int k=0; k<2; k++){
                    for(int l=0; l<4; l++){
                        if(cam2_feat[m][n][k][l] < 0){
                            cam2_feat_reshape.push_back(0);
                        }else{
                            cam2_feat_reshape.push_back(cam2_feat[m][n][k][l]);
                        }
                    }
                }
            }
        }

        float vcost_dist = vectors_distance(cam1_feat_reshape,cam2_feat_reshape);
        vcost_tmp[index] = vcost_dist;
        vnode.push_back(i/2);
        vnode.push_back(nodes[i]);
        vnode.push_back(nodes[i+1]);
        index++;
    }

    vector <int> vcost_list;
    int vcost_n = vcost_tmp.size(); 
    sortArr(vcost_tmp, vcost_n, vcost, vcost_list);


    // cout<<vcost.size();

    for(int i=0; i<count; i++){
        for(int j=0; j<3; j++){
            vlist[i][j] = vnode[vcost_list[i]*3+j];
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
                    // nodeTemp[node_index][0] = vlist[j][1];
                    // nodeTemp[node_index][1] = vlist[j][2];
                    // nodeTemp[node_index][2] = vcost[j];
                    // node_index ++;
                    nodeTemp.push_back(vlist[j][1]);
                    nodeTemp.push_back(vlist[j][2]);
                    nodeTemp.push_back(vcost[j]);

                    // edgeTemp[edge_index][0] = vlist[i][1];
                    // edgeTemp[edge_index][1] = vlist[i][2];
                    // edgeTemp[edge_index][2] = vlist[j][1];
                    // edgeTemp[edge_index][3] = vlist[j][2];
                    // edgeTemp[edge_index][4] = ecost[vlist[i][0]][vlist[j][0]];
                    // edge_index ++;
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
        
        int labels = nodeTemp.size()/3;
        if(labels > maxLabel){
            maxLabel = labels;
            float nodeCost = 0;
            float edgeCost = 0;

            for(int j=0; j<nodeTemp.size()/3; j=j+3){
                nodeCost += nodeTemp[j+2];

            }
            for(int j=0; j<edgeTemp.size()/5; j=j+5){
                edgeCost += nodeTemp[j+4];
            }

            float cost = nodeCost; 

            if(cost <= maxCost){
                maxCost = cost;
                nodeUse = nodeTemp;
                edgeUse = edgeTemp;
                break;
            }    
        }else if(labels == maxLabel){
            float nodeCost = 0;
            float edgeCost = 0;

            for(int j=0; j<nodeTemp.size()/3; j=j+3){
                nodeCost += nodeTemp[j+2];

            }
            for(int j=0; j<edgeTemp.size()/5; j=j+5){
                edgeCost += nodeTemp[j+4];
            }

            float cost = nodeCost; 

            if(cost <= maxCost){
                maxCost = cost;
                nodeUse = nodeTemp;
                edgeUse = edgeTemp;
                break;
            }    
        }
    }

    for(int i=0; i<nodeUse.size(); i=i+3){
        cout<<nodeUse[i]<<"<->"<<nodeUse[i+1]<<endl;
    }

    delete [] cam1Groups;
    delete [] cam2Groups;
    delete [] graph;
    delete [] conflict;
    delete [] ecost;
    delete [] vlist;
}
