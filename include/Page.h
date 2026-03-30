#ifndef PAGE_H
#define PAGE_H

#include <string>

class Page{
    private:
        int pageKey {}; 
        std::string content {}; 
        bool dirty {}; 
    public: 
        bool referenceBit {false};

        void clearFrame() {
            pageKey = 0;
            content = "";
            dirty = false;
            referenceBit = false;
        }

        bool isEmpty() const{
            if(content == "") return true;
            return false;
        }

        int getKey () const{
            return pageKey;
        }

        void setKey(int key){
            pageKey = key;
        }

        const std::string& getContent () const{
            return content;
        }

        void setContent(std::string cont){
            content = cont;
        }

        bool isDirty () const{
            return dirty;
        }

        void setDirty(bool dir){
            dirty = dir;
        }

};

#endif