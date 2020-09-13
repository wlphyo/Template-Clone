// $Id: main.cpp,v 1.11 2018-01-25 14:19:29-08 - - $
/*
 William Phyo (wlphyo@ucsc.edu)
Yuance Lin (ylin198@ucsc.edu) */
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <regex>
#include <unistd.h>
#include <fstream>
using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"

using str_str_map = listmap<string,string>;
using str_str_pair = str_str_map::value_type;

void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            debugflags::setflags (optarg);
            break;
         default:
            complain() << "-" << char (optopt) << ": invalid option"
                       << endl;
            break;
      }
   }
}

int main (int argc, char** argv) {
   //TA NOTES
   regex empty_regex {R"(^\s*?$)"};
   regex comment_regex {R"(^\s*(#.*)?$)"}; 
   regex key_value_regex {R"(^\s*(.*?)\s*=\s*(.*?)\s*$)"};
   regex key_regex {R"(^\s*(.*)\s*?$)"};
  // regex trimmed_regex {R"(^\s*([^=]+?)\s*$)"};
   regex equals_value_regex {R"(^\s*[=]\s*(.*?)\s*$)"};
   regex key_equals_regex {R"(^\s*(.*)\s*=\s*$)"};
   regex equals_regex {R"(^\s*[=]\s*$)"};

   
   
   
   sys_info::execname (argv[0]);
   scan_options (argc, argv);
   fstream input;
   smatch res; //iterator from regex lib
   int linenum = 0;
   str_str_map strmap;
   string fname,fileline;
   //when no files are specified
   if (argv[1] == NULL)
   { 
       while(getline(cin,fileline))
       {
          linenum++;
          if(regex_search(fileline,res,comment_regex))
          {

             cout<<"-:"<<linenum<<" : "<<res[1]<<endl;
             continue;
          }
          if(regex_search(fileline,res,empty_regex))
          {
             cout<<"-: "<<linenum<<": \n";
             continue;
          }
          if(regex_search(fileline,res,equals_regex))
          {
              cout<<"-: "<<linenum<<": ="<<endl;

              for(auto walker = strmap.begin(); walker!=strmap.end();
                  ++walker)
              {
                  cout<<walker->first<<" = "<<walker->second<<endl;
              }
              continue;
          }           
          if(regex_search(fileline,res,equals_value_regex))
          {
              cout<<"-: "<<linenum<<": = "<< res[1]<<endl;
              str_str_map toPrint = strmap.collect_k(res[1]);
              for(auto walker = toPrint.begin();
                  walker!=toPrint.end();
                  ++walker)
              {
                  cout<<walker->first<<" = "<<walker->second<<endl;
              }
              continue;
          }
          if(regex_search(fileline,res,key_value_regex))
          { 

              cout<<"-: "<<linenum<<": "<<res[1]<<"="<<res[2]<<endl;
              cout<<res[1]<<" = " <<res[2]<<endl;
              //need string string map to find key
              str_str_pair temp(res[1],res[2]);
              strmap.insert(temp);
              continue;
          }
          if(regex_search(fileline,res,key_equals_regex))
          {
              cout<<"-: "<<res[1]<<"=: "<<linenum <<": \n";
              if(strmap.find(res[1]) != strmap.end())
              {
                  strmap.erase(strmap.find(res[1]));
                  continue;
              }
              else
              {
                 cout<<res[1]<<": key not found\n";
              } 
          }
          if(regex_search(fileline,res,key_regex))
          {
              cout<<"-: "<<linenum<<": "<<res[1]<<"\n";
              if(strmap.find(res[1]) != strmap.end())
              {
                  auto toAccess = strmap.find(res[1]);
                  cout<<toAccess->first<<" = "<<toAccess->second<<endl;
                  continue;
              }
              else
              {
                 cout<<res[1]<<": key not found\n";
              } 
          }
       }
   }else
   {   
      for(int index = 1; index< argc; index++)
      {
         input.open(argv[index]);
         fname = argv[index];
         if(input.is_open())
         {
           // string fileline;
            
            while(getline(input,fileline))
            {
               linenum++;
          if(regex_search(fileline,res,comment_regex))
          {

             cout<<fname<<": "<< linenum<<": "<<res[1]<<endl;
             continue;
          }
          if(regex_search(fileline,res,empty_regex))
          {
             cout<<fname<<": "<<linenum<<" : \n";
             continue;
          }
          if(regex_search(fileline,res,equals_regex))
          {
              cout<<fname<<": "<<linenum<<": ="<<endl;

              for(auto walker = strmap.begin(); walker!=strmap.end();
                  ++walker)
              {
                  cout<<walker->first<<" = "<<walker->second<<endl;
              }
              continue;
          }           
          if(regex_search(fileline,res,equals_value_regex))
          {
              cout<<fname<<": "<<linenum<<": = "<< res[1]<<endl;
              str_str_map toPrint = strmap.collect_k(res[1]);
              for(auto walker = toPrint.begin();
                  walker!=toPrint.end();
                  ++walker)
              {
                  cout<<walker->first<<" = "<<walker->second<<endl;
              }
              continue;
          }
          if(regex_search(fileline,res,key_value_regex))
          { 

              cout<<fname<<": "<<linenum<<": "
                  <<res[1]<<"="<<res[2]<<endl;
              cout<<res[1]<<" = " <<res[2]<<endl;
              //need string string map to find key
              str_str_pair temp(res[1],res[2]);
              strmap.insert(temp);
              continue;
          }
          if(regex_search(fileline,res,key_equals_regex))
          {
              cout<<fname<<": "<<res[1]<<"=: "<<linenum <<": \n";
              if(strmap.find(res[1]) != strmap.end())
              {
                  strmap.erase(strmap.find(res[1]));
                  continue;
              }
              else
              {
                 cout<<res[1]<<": key not found\n";
              } 
          }
          if(regex_search(fileline,res,key_regex))
          {
              cout<<fname<<": "<<linenum<<": "<<res[1]<<"\n";
              if(strmap.find(res[1]) != strmap.end())
              {
                  auto toAccess = strmap.find(res[1]);
                  cout<<toAccess->first<<" = "<<toAccess->second<<endl;
                  continue;
              }
              else
              {
                 cout<<res[1]<<": key not found\n";
              } 
          }
            }
             input.close();
             linenum=0;
         }else
         {
             cout<<"keyvalue: "<<fname<<": No such file or directory\n";
         }
      }
     
   }
   
   
   return EXIT_SUCCESS;
}

