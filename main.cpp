#include "src/forward_index.hpp"
#include <filesystem>

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

void read_inverted(unordered_map<string,node*> & mp)
{
  ifstream inverted("inverted.txt");
  char c;
  string word, id, score;
  node *temp;
  c = inverted.get();
  while (c != EOF)
  {
    // New Word Detected
    if (c == '!')
    {
      word.clear();
      // Finding whole world
      while ((c = inverted.get()) != '\\' && c != ':')
      {
        word += c;
      }
      mp[word] = nullptr;
    }
    // New ID
    if (c == '\\')
    {
      id.clear();
      score.clear();
      // Finding whole id
      while ((c = inverted.get()) != ':')
      {
        id += c;
      }
      // Finding it's score
      while ((c = inverted.get()) != '\\' && c != '!' && c!=EOF)
      {
        score += c;
      }
      // Adding node to linked list
      node *temp2 = new node(stoi(id), stoi(score));
      if (mp[word] == nullptr)
      {
        mp[word] = temp2;
        temp = temp2;
      }
      else
      {
        temp->next = temp2;
        temp = temp2;
      }
    }
  }
  return;
}

void inverted_index(string a, unordered_map<string, node *> &mp)
{
  ifstream forward(a);
  if (!forward.is_open())
  {
    cout << "Can Not Open Forward index file";
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
      while ((c = forward.get()) != '\\' && c != '!')
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
      node *temp = new node(stoi(id), stoi(score));
      if (mp[word] == nullptr)
      {
        mp[word] = temp;
      }
      else
      {
        node *temp2 = mp[word];
        while (temp2->next != nullptr)
        {
          temp2 = temp2->next;
        }
        temp2->next = temp;
      }
    }
  }
  forward.close();
  //Writing to inverted.txt file whole inverted index
  ofstream inverted("inverted.txt");
  node*temp;
  for (auto x : mp) {
      inverted << '!' << x.first;
      temp=x.second;
      while(temp!=nullptr){
        inverted<<'\\'<<temp->id<<':'<<temp->score;
        temp=temp->next;
      }
  }
  inverted.close();
}

void search_title(vector<int>&a,vector<vector<string>>&b){
  ifstream file("ForwardIndex\\metadata.txt");
  if (!file.is_open())
  {
    cout << "Can Not Open meta data file";
    return;
  }
  
  char c=file.get();string id,title,url;
  while (c!=EOF) {
        if (c == '!') {
            id.clear();
            while (file.get(c) && c != '\\') {
                id += c;
            }
            for (int i = 0; i < a.size(); i++) {
                if (stoi(id) == a[i]) {
                    title.clear();
                    while (file.get(c) && c != ':') {
                        title += c;
                    }
                    url.clear();
                    while (file.get(c) && c != '!') {
                        url += c;
                    }
                    b[i].push_back(title);
                    b[i].push_back(url);
                    break;
                }
                if(i==a.size()-1){
                  while(c!='!'&&c!=EOF){c=file.get();}
                }
            }
        }
    }
    file.close(); 
}

void read_title(){
  vector<int>a;vector<vector<string>>b;
  a.push_back(0);
  a.push_back(1);
  vector<string>d;
  for(int i=0;i<a.size();i++){
    b.push_back(d);
  }
  search_title(a,b);
  cout << endl << "Size of b: " << b.size() << endl;
    for (int i = 0; i < b.size(); i++) {
        cout << "Element " << i << ":\n";
        for (int j = 0; j < b[i].size(); j++) {
            cout << b[i][j] << "\n";
        }
    }
}

void search_words(vector<string>words,unordered_map<string,node*> & mp){
  
}
int main()
{
  unordered_map<string,node*>mp;
  auto start_time = std::chrono::high_resolution_clock::now();
  // forwardIndex("369news.json");
  // inverted_index("ForwardIndex\\Index0.txt",mp);
  // read_inverted(mp);
  //Checking loading inverted index
  // node* temp3;
  // for (auto x : mp) {
  //     cout << '!' << x.first;
  //     temp3=x.second;
  //     while(temp3!=nullptr){
  //       cout<<'\\'<<temp3->id<<':'<<temp3->score;
  //       temp3=temp3->next;
  //     }
  // }
  // read_forward("index");
  read_title();
  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

  std::cout << "Time taken by the first part: " << duration.count() / 1000000 << "seconds" << std::endl;
}
