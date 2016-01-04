//
// Created by jiadongyu on 1/4/16.
//

#ifndef LTP_LANGUAGE_TECHNOLOGY_PLATFORM_PREDICT_H
#define LTP_LANGUAGE_TECHNOLOGY_PLATFORM_PREDICT_H

#include "genfeature.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

class Predictor{
    const string mInputFilePath;
    const string mOutputFilePath;

    const string tmpFile="svm/tmp_data/feature.txt";
    const string rawFile="svm/tmp_data/raw_data.txt";

    std::unordered_set<string> nation;

    ofstream tmpOfStream;

    XMLDocument xmlDocument;
    Model model;
public:
    int main(){
        ifstream fin("data/nation.txt");
        string tmp;
        if(!fin.good()){
            return -1;
        }
        while(fin>>tmp){
            nation.insert(tmp);
        }
        fin.close();

        loadFile();
        GetAllFeature();
    }

    Predictor(): mInputFilePath("data/xml/input.xml"), mOutputFilePath("data/xml/output.xml"){
    }

    int loadFile() {
        cerr<<mInputFilePath.c_str()<<endl;
        cerr<<mInputFilePath<<endl;
        int rtn =xmlDocument.LoadFile(mInputFilePath.c_str());
        CHECK_RTN_LOGE(rtn,"load error:"+ mInputFilePath);
        return 0;
    }

    int saveFile() {
        int rtn =xmlDocument.SaveFile(mOutputFilePath.c_str());
        CHECK_RTN_LOGE(rtn,"save error");
        return 0;
    }

    int GetAllFeature(){
        ofstream tmpOfStream(tmpFile);
        ofstream rawOfStream(rawFile);

        if(!tmpOfStream.good()){
            cerr<<"tmp file not good"+ tmpFile<<endl;
        }else{
            cerr<<"tmp file good"<<endl;
        }
        cerr<<"succ constrcut: "<<mInputFilePath<<endl;

        XMLElement *weibos=xmlDocument.RootElement();
        XMLElement *weibo=weibos->FirstChildElement("weibo");
        while(weibo){
            string id;
            string text;
            const XMLAttribute *attributeOfWeibo = weibo->FirstAttribute();
            cout<< attributeOfWeibo->Name() << ":" << attributeOfWeibo->Value() << endl;
            id=attributeOfWeibo->Value();
            text= weibo->GetText();
            cout<<id<<" "<<text<<endl;
            int rtn =0;
            rtn = GetSentenceFeature(text,id, tmpOfStream, rawOfStream);
            CHECK_RTN_LOGE_CTN(rtn,"GetSenteceFeature error");
            weibo=weibo->NextSiblingElement();
        }
        return 0;
    }

    int GetSentenceFeature(const string & sentence,  const string & id, ofstream & tmpOfStream, ofstream & rawOfStream) const{
        vector<string> words, post_tags,nes;
        vector<pair<int,string>> parseTree;
        int rtn =0;
        rtn = model.parse(sentence,words,post_tags,nes,parseTree);
        CHECK_RTN_LOGE(rtn,"parse error");
        cerr<<"parse succ!"<<endl;
        vector<int> ins ,people;
        set<int> PosP,PosI;
        for(int i=0;i<nes.size();i++){
            if(nes[i]=="S-Ni" || nes[i]=="E-Ni" || (nes[i]=="S-Ns" && nation.count(words[i])>0) ){
                ins.push_back(i);
                PosI.insert(i);
            }else if(nes[i]=="S-Nh" || nes[i] == "E-Nh" ){
                people.push_back(i);
                PosP.insert(i);
            }
        }

        cerr<<"ins#:"<<ins.size()<<" people#"<<people.size()<<endl;

        for(int i=0; i<ins.size();i++){
            for(int j=0;j<people.size();j++){
                string feature;
                int rtn =0;
                rtn = model.getFeatureByLoc(sentence, people[j],ins[i],words,post_tags,nes,parseTree,PosP,PosI,feature);
                CHECK_RTN_LOGE_CTN(rtn,"feature get by loc error at"+ ins[i]+people[j]);
                tmpOfStream<< id <<" "<<feature<<endl;

                string allp, alli;
                int k;
                for(k=ins[i]-1;k>=0;k--){
                    //cerr<<nes[k]<<endl;
                    if(nes[k]=="B-Ni" || nes[k]=="I-Ni"){
                        continue;
                    }
                    break;
                }
                for(k++;k<=ins[i];k++){
                    alli.append(words[k]);
                }

                for(k=people[j]-1;k>=0;k--){
                    //cerr<<nes[k]<<endl;
                    if(nes[k]=="B-Nh" || nes[k]=="I-Nh"){
                        continue;
                    }
                    break;
                }
                for(k++;k<=people[j];k++){
                    allp.append(words[k]);
                }
                rawOfStream<< id <<"*"<<alli<<"*"<<allp<<endl;
            }
        }
        return 0;
    }

    ~Predictor(){};



};


#endif //LTP_LANGUAGE_TECHNOLOGY_PLATFORM_PREDICT_H
