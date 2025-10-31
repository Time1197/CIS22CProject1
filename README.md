# CIS22CProject1
## Summary
This project is a rudimentary search engine that indexes all .txt files in the **documents** folder and adds them to an inverted index. The user than can search for specific queries, view the documents, and go back and forth between viewed document history in the terminals

## How To Use
1. First, place all documents you want the program to read within the **documents** file. It should be at CIS22CProject1\documents. **This file must be a .txt file**
2. Make sure the language standard is set to C++17, run the program within your IDE (I used visual studio 2022)
3. The program should be able to identify and process all .txt files within the documents folders. If not, it is likely an enviromental error
4. type 'help' in the terminal to recieve a list of commands. These are the commands:
  * query <term1> [term2]  -  search for documents containing all items. Order matters.
    * For example, if you have file.txt have contents: a b c and you search a b, you will get a result. However if you search b a, you will not get a result.
  * view <docID>  -  View the full contents of a document (e.g. view doc1.txt)
  * back  -  Go to the previously viewed document
  * forward  -  Go to the next document in history
  * help  -  show the options of possible commands
  * quit  -  exit the program
