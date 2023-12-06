// Creator: Muhammad Bilal
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>
#include <iomanip>
#include <chrono>

using namespace std;
void create()
{
    fstream fout;
    int i, ID;
    string name, type, area, link_to_regiter;
    double amount;

    cout << "Enter the ID" << endl;
    cin >> ID;
    string fname= "F:\\University\\Semester 3\\DSA\\SGC Project\\collection\\";
    fname += "0"+to_string(ID);
    fname += ".csv";
    fout.open(fname, ios::out | ios::app);
    // Read the input

    cout << "Enter the details of Scholarship/Grant:"
         << " name, Type, field, amount and Link to registration" << endl;
    cin >> name >> type >> area >> amount >> link_to_regiter;

    // Insert the data to file
    fout << ID << ", "
         << name << ", "
         << type << ", "
         << area << ", "
         << amount << ", "
         << link_to_regiter
         << "\n";
}

void delete_file(const string& fileName) {
    string filePath = "F:\\University\\Semester 3\\DSA\\SGC Project\\collection\\" + fileName;  

    if (DeleteFile(filePath.c_str())) {
        cout << "File deleted: " << fileName << endl;
    } else {
        DWORD error = GetLastError();
        cerr << "Error " << error << ": Unable to delete file" << endl;   
    }

}
void Read_File(const string file_name)
{
    string filePath = "F:\\University\\Semester 3\\DSA\\SGC Project\\collection\\"+file_name;  // Use backslash for Windows path
   
    fstream fin; 
    string line,temp;
  
    fin.open(filePath, ios::in);
    while(fin){
        getline(fin,line);
        cout<<line<<endl;
    }
}

class FileNode
{
public:
    string file_name;
    int size; 
    time_t created_at;

    FileNode(string name, int s, time_t created) : file_name(name), size(s), created_at(created) {}
};

class BTreeNode
{
public:
    bool leaf;
    vector<FileNode> keys;
    vector<BTreeNode *> children;

    BTreeNode(bool is_leaf = true) : leaf(is_leaf) {}

    ~BTreeNode()
    {
        for (auto child : children)
        {
            delete child;
        }
    }
};

class BTree
{
public:
    BTreeNode *root;
    int degree;

    void insertNonFull(BTreeNode *x, const FileNode &file_node)
    {
        int i = x->keys.size() - 1;

        if (x->leaf)
        {
            x->keys.push_back(FileNode("", 0, 0)); // Placeholder for the new key
            while (i >= 0 && file_node.created_at < x->keys[i].created_at)
            {
                x->keys[i + 1] = x->keys[i];
                i--;
            }
            x->keys[i + 1] = file_node;
        }
        else
        {
            while (i >= 0 && file_node.created_at < x->keys[i].created_at)
            {
                i--;
            }
            i++;
            if (x->children[i]->keys.size() == (2 * degree) - 1)
            {
                splitChild(x, i);
                if (file_node.created_at > x->keys[i].created_at)
                {
                    i++;
                }
            }
            insertNonFull(x->children[i], file_node);
        }
    }
    void splitChild(BTreeNode *x, int i)
    {
        int degree = this->degree;
        BTreeNode *y = x->children[i];
        BTreeNode *z = new BTreeNode(y->leaf);
        x->children.insert(x->children.begin() + i + 1, z);
        x->keys.insert(x->keys.begin() + i, y->keys[degree - 1]);
        z->keys = vector<FileNode>(y->keys.begin() + degree, y->keys.end());
        y->keys = vector<FileNode>(y->keys.begin(), y->keys.begin() + degree - 1);
    }

    BTree(int deg) : root(nullptr), degree(deg) {}

    ~BTree()
    {
        delete root;
    }

    void insert(const FileNode &file_node)
    {
        if (!root)
        {
            root = new BTreeNode(true);
            root->keys.push_back(file_node);
        }
        else
        {
            if (root->keys.size() == (2 * degree) - 1)
            {
                BTreeNode *new_root = new BTreeNode(false);
                new_root->children.push_back(root);
                splitChild(new_root, 0);
                root = new_root;
            }
            insertNonFull(root, file_node);
        }
    }
    void populateFromFolder(const string &folderPath)
    {
        WIN32_FIND_DATA findFileData;
        HANDLE hFind = FindFirstFile((folderPath + "\\*").c_str(), &findFileData);

        if (hFind == INVALID_HANDLE_VALUE)
        {
            cerr << "Error opening directory: " << folderPath << endl;
            return;
        }

        int fileNumber = 1;

        do
        {
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                continue; // Skip folders
            }

            ostringstream oss;
            oss << setw(3) << setfill('0') << fileNumber++;
            string fileName = oss.str() + ".csv";

            FileNode newFile(fileName, sizeof(fileName), 0); 
            insert(newFile);

        } while (FindNextFile(hFind, &findFileData) != 0);

        FindClose(hFind);
    }
    void displayFileNames(BTreeNode *node)
    {
        if (node != nullptr)
        {
            for (const auto &file : node->keys)
            {
                cout << file.file_name <<" Size:"<< file.size<<endl;
            }

            if (!node->leaf)
            {
                for (auto child : node->children)
                {
                    displayFileNames(child);
                }
            }
        }
    }
    bool searchFile(const string &file_name, BTreeNode *node)
    {
        if (node != nullptr)
        {
            for (const auto &file : node->keys)
            {
                if (file.file_name == file_name)
                {
                    cout << "File found: " << file_name << endl;
                    return true;
                }
            }

            if (!node->leaf)
            {
                for (auto child : node->children)
                {
                    if (searchFile(file_name, child))
                    {
                        return true;
                    }
                }
            }
        }

        return false;
    }

    bool searchFile(string &file_name)
    {
        file_name ="0"+file_name+".csv";
        return searchFile(file_name, root);
    }
};

int main()
{
    cout<<"		()()()()  ()()()  ()    ()   ()()   ()       ()()    ()()   ()()()() ()    () ()()()() ()()() "<<endl<<
        "		()       ()       ()    () ()    () ()     ()    () ()   () ()       ()    ()    ()    ()   ()"<<endl<<
        "		()()()() ()       ()()()() ()    () ()     ()()()() ()()()  ()()()() ()()()()    ()    ()()() "<<endl<<
        "		      () ()       ()    () ()    () ()     ()    () ()  ()        () ()    ()    ()    ()     "<<endl<<
        "		()()()()  ()()()  ()    ()   ()()   ()()() ()    () ()   () ()()()() ()    () ()()()() ()     "<<endl<<endl<<
        "		          ()()()     ()()     ()()    ()    () ()()()()        ()()   ()()()  ()()()  "<<endl<<
        "		         ()         ()   () ()    () () ()  ()    ()         ()    () ()   () ()   () "<<endl<<
        "		         ()  ()()() ()()()  ()()()() ()  () ()    ()         ()()()() ()()()  ()()()  "<<endl<<
        "		         ()  ()  () ()  ()  ()    () ()   ()()    ()         ()    () ()      ()      "<<endl<<
        "		          ()()   () ()   () ()    () ()    ()     ()         ()    () ()      ()      "<<endl<<
        "\n\n\t\t\tGROUP MEMBERS:"<<
        "\n\n\t\t\tAsghar Ali      (22k-4415)"<<
        "\n\n\t\t\tAbdullah Shafiq (22k-4489)"<<
        "\n\n\t\t\tMuhammad Bilal  (22k-4242)"<<endl;
    
    BTree btree(8);

    // Populating B-tree from a Windows folder
    btree.populateFromFolder("F:\\University\\Semester 3\\DSA\\SGC Project\\collection");

    string optionNumber;
    while (1)
    {
        cout << "\n\n\t\t\t````````` The Main Menu `````````" << endl;
        cout << "\t\t\t---------------------------------" << endl;
        cout << "\t\t\t1.Add a new scholarship \n\t\t\t2.Delete a Scholarship \n\t\t\t3.Modify \n\t\t\t4.Find a scholarship \n\t\t\t5.List Files \n\t\t\t6.Exit\n";
        cout << "\t\t\t---------------------------------" << endl;
        cout << "\n\t\t\tEnter The Option Number: ";
        cin >> optionNumber;
        cout << endl;

        if (optionNumber == "1")
        {
            create();
            cout << "Scholarship/Grant created successfully, Restart program to update collection" << endl;

        }
        else if (optionNumber == "2")
        {
            string file_to_delete;
            cout << "Enter the ID of Scholarship to search(i.e 35): ";
            cin >> file_to_delete;
            if (btree.searchFile(file_to_delete))
            {
                delete_file(file_to_delete);
                cout << "Scholarship/Grant Deleted successfully, Restart program to update collection" << endl;

            }
            else
            {
                cout << "Does not exist" << endl;
            }


        }
        else if (optionNumber == "3")
        {
            string mod_file_name;
            cout << "Enter the ID of Scholarship to search (i.e 26): ";
            cin >> mod_file_name;
            if (btree.searchFile(mod_file_name))
            {
                delete_file(mod_file_name);
                create();
            }
            else
            {
                cout << "Does not exist" << endl;
            }
        }
        else if (optionNumber == "4")
        {
            string search_file_name;
            cout << "Enter the ID of Scholarship to search (i.e 18): ";
            cin >> search_file_name;
            auto start = chrono::high_resolution_clock::now();

            if (btree.searchFile(search_file_name))
            {
                cout << "found!" << endl;
            }
            else
            {
                cout << "Does not exist" << endl;
            }
            auto stop = chrono::high_resolution_clock::now();

            auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
            
            Read_File(search_file_name);
            
            cout << "Time taken to search file: " << duration.count() << " microseconds." << endl;

        }
        else if (optionNumber == "5")
        {
            cout << "File names in B-tree:" << endl;
            btree.displayFileNames(btree.root);
        }
        else if (optionNumber == "6")
        {
            break;
        }
        else if (optionNumber == "dev")
        {
            break;
        }
        else
        {
            cout << "Invalid Input.! Please Try Again." << endl;
        }
    }

    return 0;
}