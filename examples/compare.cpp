//
// Created by jiadongyu on 1/4/16.
//

#include "iostream"
#include "fstream"
#include "sstream"
#include "vector"
#include "unordered_map"
#include "set"
#include "tinyxml2.h"


using namespace std;
using namespace tinyxml2;

int main(){
    ifstream rawDataStream("svm/tmp_data/raw_data.txt");
    ifstream resultStream("svm/tmp_data/output.txt");

    const string xml = "data/xml/input.xml";
    const string xml_output="data/xml/output.xml";
    unordered_map<string,unordered_map<string,string> > mp;
    unordered_map<string,unordered_map<string,double>> score;
    vector<double> result;
    double dou;
    result.reserve(5000);
    while(resultStream>>dou){
        result.push_back(dou);
    }
    vector<string> rawData;
    rawData.reserve(5000);
    string tmp;
    while(rawDataStream>>tmp){
        rawData.push_back(tmp);
    }

    if(rawData.size()!=result.size()){
        cerr<<"############# size not same raw:"<< rawData.size()<<" result: "<<result.size();
    }

    for(int i=0;i<result.size();i++){
        if(result[i]>0){
            stringstream ss(rawData[i]);
            string id,ins,people;
            getline(ss, id, '*');
            getline(ss, ins, '*');
            ss >> people;
            if(result[i]>score[id][people]) {
                cerr<<result[i]<<endl;
                mp[id][people]=ins;
                score[id][people]=result[i];
            }
        }
    }

    XMLDocument xmlDocument;
    xmlDocument.LoadFile(xml.c_str());
    XMLElement *weibos=xmlDocument.RootElement();
    XMLElement *weibo=weibos->FirstChildElement("weibo");
    while(weibo){
        string id;
        string text;
        const XMLAttribute *attributeOfWeibo = weibo->FirstAttribute();
        cout<< attributeOfWeibo->Name() << ":" << attributeOfWeibo->Value() << endl;
        id=attributeOfWeibo->Value();
        text= weibo->GetText();
        //cout<<id<<" "<<text<<endl;
        cerr<<id<<"#"<<text<<endl;
        for(auto a:mp[id]) {
            XMLElement *ele = xmlDocument.NewElement("employee");
            ele->SetAttribute("from",a.second.c_str());
            cerr<<"from: "<< a.second;
            ele->SetAttribute("name",a.first.c_str());
            weibo->InsertEndChild(ele);
            cerr<<"hire as: "<<a.first<<endl;
        }
        weibo=weibo->NextSiblingElement();
    }
    xmlDocument.SaveFile(xml_output.c_str());


    return 0;
}
