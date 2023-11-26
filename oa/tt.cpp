#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <windows.h>
#include <iomanip>

using namespace std;

struct Node
{
    string name;
    vector<Node *> children;

    Node(string n) : name(n) {}
};

bool traverse(Node *current, const string &destination, vector<string> &path)
{
    if (current->name == destination)
    {
        path.push_back(current->name);
        return true;
    }

    for (Node *child : current->children)
    {
        if (traverse(child, destination, path))
        {
            path.insert(path.begin(), current->name + "/");
            return true;
        }
    }

    return false;
}

void createFolderOrFile(Node *root, const vector<string> &path, const string &choice)
{
    Node *current = root;

    for (const string &node : path)
    {
        for (Node *child : current->children)
        {
            if (child->name == node)
            {
                current = child;
                break;
            }
        }
    }

    if (choice == "folder")
    {
        string folderName;
        cout << "Enter the name of the folder: ";
        cin >> folderName;

        Node *newNode = new Node(folderName);

        current->children.push_back(newNode);

        string folderPath = "";
        for (const string &node : path)
        {
            folderPath += node + "/";
        }
        folderPath += folderName;

        if (CreateDirectory(folderPath.c_str(), NULL) != 0)
        {
            cout << "Folder '" << folderName << "' added at path: " << folderPath << endl;
        }
        else
        {
            cerr << "Failed to create folder '" << folderName << "' at path: " << folderPath << endl;
        }
    }
    else if (choice == "file")
    {
        string fileName;
        cout << "Enter the name of the file (without extension): ";
        cin >> fileName;

        Node *newNode = new Node(fileName);

        current->children.push_back(newNode);

        fileName += ".txt";

        string fileContent;
        cout << "Enter the content of the file: ";
        cin.ignore();
        getline(cin, fileContent);

        string filePath = "";
        for (const string &node : path)
        {
            filePath += node + "/";
        }
        filePath += fileName;

        ofstream outFile(filePath, ios_base::app);
        outFile << fileContent;
        outFile.close();

        cout << "File '" << fileName << "' created at path: " << filePath << endl;
    }
}

// void getNodeContents(Node *current)
// {
//     cout << "Contents of node '" << current->name << "':" << endl;
//     for (Node *child : current->children)
//     {
//         cout << child->name << endl;
//     }
// }
void displayDirectoryContents(const string &path)
{
    WIN32_FIND_DATA findFileData;
    HANDLE hFind;

    string fullpath = path + "\\*";

    hFind = FindFirstFile(fullpath.c_str(), &findFileData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        cout << "No files found\n";
    }
    else
    {
        cout << "Files in directory: " << path << endl;
        do
        {
            // Ignore the current and parent directories
            string filename = findFileData.cFileName;
            if (filename != "." && filename != "..")
            {
                cout << filename << endl;
            }
        } while (FindNextFile(hFind, &findFileData));
        FindClose(hFind);
    }
}

int main()
{
    // Build the tree
    Node *pc = new Node("pc");
    Node *disk1 = new Node("disk1");
    Node *disk2 = new Node("disk2");
    Node *f1 = new Node("f1");
    Node *f2 = new Node("f2");
    Node *f3 = new Node("f3");
    Node *f4 = new Node("f4");
    Node *f5 = new Node("f5");
    Node *f6 = new Node("f6");

    pc->children = {disk1, disk2};
    disk1->children = {f1, f2, f3};
    disk2->children = {f4, f5, f6};

    cout << "Intinally we have" << endl;
    cout << "pc->children = {disk1, disk2}" << endl;
    cout << "disk1->children = {f1, f2, f3}" << endl;
    cout << "disk2->children = {f4, f5, f6}" << endl;
    cout << endl;

    while (true)
    {
        string pathString;
        cout << "Enter a path (e.g., pc/disk1/f2): ";
        cin >> pathString;

        if (pathString == "4")
        {
            break;
        }

        vector<string> path;
        istringstream ss(pathString);
        string node;
        while (getline(ss, node, '/'))
        {
            path.push_back(node);
        }

        Node *currentNode = pc;
        cout << "[" << pc->name;
        for (const string &nodeName : path)
        {
            for (Node *child : currentNode->children)
            {
                if (child->name == nodeName)
                {
                    cout << "/" << child->name;
                    currentNode = child;
                    break;
                }
            }
        }
        cout << "]$ ";

        cout << "Options:" << endl;
        cout << "1 - Create Folder" << endl;
        cout << "2 - Create File" << endl;
        cout << "3 - Show Contents" << endl;
        cout << "4 - Exit" << endl;

        int userChoice;
        cout << "Enter your choice: ";
        cin >> userChoice;

        switch (userChoice)
        {
        case 1:
            createFolderOrFile(pc, path, "folder");
            break;
        case 2:
            createFolderOrFile(pc, path, "file");
            break;
        case 3:
        {
            string fullPath = "";
            for (const string &node : path)
            {
                fullPath += node + "/";
            }
            displayDirectoryContents(fullPath);
        }
            break;
        
        case 4:
            break;
        default:
            cout << "Invalid choice. Please enter a number between 1 and 4." << endl;
            break;
        }
    }

    delete pc;
    delete disk1;
    delete disk2;
    delete f1;
    delete f2;
    delete f3;
    delete f4;
    delete f5;
    delete f6;

    return 0;
}