#include "src/forward_index.hpp"
#include <filesystem>
#include<stdexcept>

using namespace std;
class node
{
public:
  int id;
  int score;
  node *next;
  node(int ID, int Score)
  {
    id = ID;
    score = Score;
    next = nullptr;
  }
};
class LL{
  public:
  node*head;
  node*tail;
  LL(){
    head=nullptr;tail=nullptr;
  }
  void insert(int id,int score){
    if(tail==nullptr){
      head=new node(id ,score);
      tail=head;
    }
    else{
      tail->next=new node(id,score);
      tail=tail->next;
    }
  }
};
class pairs{
  public:
  int id;
  int score;
  pairs(int ID, int Score)
  {
    id = ID;
    score = Score;
  }
};
class str_pair{
  public:
  string title;
  string url;
  int score;
  str_pair(string a, string b, int s) : title(a), url(b), score(s) {}
};
// Custom hash function for strings using the djb2 algorithm
size_t customHash(const std::string& key) {
    size_t hash = 5381;

    for (char c : key) {
        hash = ((hash << 5) + hash) + static_cast<size_t>(c); // djb2 hash algorithm
    }

    return hash;
}

void read_inverted(unordered_map<string,LL, decltype(&customHash)>& mp)
{
  ifstream inverted("inverted.txt");
  if(!inverted.is_open()){
    return;
  }
  char c;
  string word, id, score;
  c = inverted.get();
  while (c != EOF)
  {
    // New Word Detected
    if (c == '!')
    {
      word.clear();
      // Finding whole world
      while ((c = inverted.get()) != '\\' && c != ':'){
        word += c;
      }
      mp[word].head = nullptr;
    }
    // New ID
    if (c == '\\')
    {
      id.clear();
      score.clear();
      // Finding whole id
      while ((c = inverted.get()) != ':'){
        id += c;
      }
      // Finding it's score
      while ((c = inverted.get()) != '\\' && c != '!' && c!=EOF){
        score += c;
      }
      // Adding node to linked list
      mp[word].insert(stoi(id), stoi(score));
    }
  }
  inverted.close();
  return;
}

void inverted_index(string a, unordered_map<string,LL, decltype(&customHash)>& mp)
{
  ifstream forward(a);
  if (!forward.is_open())
  {
    std::cout << "Can Not Open Forward index file";
    return;
  }
  char c = forward.get();
  string id, word, score;
  while (c != EOF)
  {
    //new document detected
    if (c == '!')
    {
      id.clear();
      //getting full id
      while ((c = forward.get()) != '\\')
      {
        id += c;
      }
    }
    // Word detection
    if (c == '\\')
    {
      word.clear();
      while ((c = forward.get()) != ':')
      {
        word += c;
      }
      score.clear();
      while ((c = forward.get()) != '\\' && c != '!'&& c!=EOF)
      {
        score += c;
      }
      //storing word and its score
      //cout<<score<<id;
      mp[word].insert(stoi(id), stoi(score));
    }
  }
  forward.close();
  //Writing to inverted.txt file whole inverted index
  ofstream inverted("inverted.txt");
  node*temp;
  for (auto& x : mp) {
      inverted << '!' << x.first;
      temp=x.second.head;
      while(temp!=nullptr){
        inverted<<'\\'<<temp->id<<':'<<temp->score;
        temp=temp->next;
      }
  }
  inverted.close();
}

void search_title(vector<pairs*>&a,vector<str_pair>&b){
  ifstream file("ForwardIndex\\metadata.txt");
  if (!file.is_open())
  {
    std::cout << "Can Not Open meta data file";
    return;
  }
  str_pair d("","",0);
  for(int i=0;i<a.size();i++){
    b.push_back(d);
  }
  int article_id=a[0]->id;
  char c=file.get();string id,title,url;int j=0;
  while (!file.eof()) {
        if (c == '`') {
            id.clear();
            while (file.get(c) && c != '\\') {
                id += c;
            }
            if(article_id==stoi(id)){
              title.clear();
              while (file.get(c) && c != ':') {
                title += c;
              }
              url.clear();
              while (file.get(c) && c != '`') {
                url += c;
              }
              b[j].title=title;
              b[j].url=url;
              b[j].score=a[j]->score;
              j++;
              if(j==a.size()){return;}
              article_id=a[j]->id;
            }
            else{
                  while(c!='`'&&c!=EOF){c=file.get();}
            }
        }
    }
    file.close(); 
}

bool check_nullptr(vector<node*>&lists){
  for(int i=0;i<lists.size();i++){
    if(lists[i]==nullptr){return true;}
  }
  return false;
}

void greater_or_equal_tomax(vector<node*>&lists,int max){
  for(int i=0;i<lists.size();i++){
    while( lists[i]!=nullptr && lists[i]->id<max){
      lists[i]=lists[i]->next;
    }
  }
}

int Partition(vector<str_pair> &v, int start, int end) {
    int pivot = end;
    int j = start;
    for (int i = start; i < end; ++i) {
        if (v[i].score > v[pivot].score) {
            swap(v[i], v[j]);
            ++j;
        }
    }
    swap(v[j], v[pivot]);
    return j;
}

void Quicksort(vector<str_pair> &v, int start, int end ){

	if(start<end){
		int p = Partition(v,start,end);
		Quicksort(v,start,p-1);
		Quicksort(v,p+1,end);
	}
	
}

void search_words(vector<string>words,unordered_map<string, LL, decltype(&customHash)>& mp){
  vector<node*>lists;
  for(int i=0;i<words.size();i++){
    if(mp[words[i]].head==nullptr){
      std::cout<<"\nNo Matches\n";return;
    }
    lists.push_back(mp[words[i]].head);
  }
  //Finding id's with same words
  vector<pairs*>ID;
  while(!check_nullptr(lists)){
    int id=lists[0]->id,max=-1;
    for(int i=0;i<lists.size();i++){
      if(id!=lists[i]->id){
        for( ;i<lists.size();i++){
          if(id<lists[i]->id){
            id=lists[i]->id;max=id;
          }else if(max==-1){
            max=id;
          }
        }
      }
    }
    if(max!=-1){
      greater_or_equal_tomax(lists,max);
    }else{
      int sum=0;
      for(int i=0;i<lists.size();i++){
        sum+=lists[i]->score;
      }
      ID.push_back(new pairs(lists[0]->id,sum));
      for(int i=0;i<lists.size();i++){
        lists[i]=lists[i]->next;
      }
    }
  }
  if(ID.size()==0){cout<<"No such articles";return;}
  
  cout<<"\nnear boys\n";
  //Recieving titles and urls in a vector<vector<string>>
  vector<str_pair> info;
  search_title(ID,info);
  //Sorting retrieved id's according to score
  Quicksort(info,0,info.size()-1);
  std::cout << endl << "Total no of articles: " << info.size() << endl;
    for (int i = 0; i < info.size(); i++) {
        std::cout << "\nArticle " << i+1 << ":\n";
        cout << info[i].title << "\n"<<info[i].url;
    }
}




int main()
{

  unordered_map<string, LL, decltype(&customHash)> mp(10000000, customHash);//read_inverted(mp);
  //mp.reserve(1000000);
  
  read_inverted(mp);
  
  auto start_time = std::chrono::high_resolution_clock::now();
  
  // forwardIndex("cnn.json");
  // inverted_index("ForwardIndex\\Index.txt",mp);

  vector<string>a;
  //a.push_back("india");
  a.push_back("pakistan");
  //a.push_back("india");
  search_words(a,mp);

  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

  std::cout << "\nTime taken by the first part: " << duration.count() / 1000 << "miliseconds" << std::endl;
}
