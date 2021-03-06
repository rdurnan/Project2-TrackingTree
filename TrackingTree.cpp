// TrackingTree.cpp
// Ryan Durnan
// COSC 2030
// Project 2 : Tracking Tree
// 8 Dec 2018

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::to_string;
using std::hash;
using std::pow;
using std::getline;

// Contains all the information needed in the tracking tree
// Note: Does not actually build a tree. Only holds information 
//		 for each node of the tree. A vector in main() stands
//		 in for the actual tree, built in levelorder.
class TreeInfo
{
// This is all the information in a node
protected:
	string ID;				// Hash of the parent ID and raw event
	string PID;				// Parent ID
	string rawEvent;		// Data being stored
	string lHash;			// hash(left child contents)
	string rHash;			// hash(right child contents)
	vector<string> lHist;	// Vector of 8-character strings. Tracks left history
	vector<string> rHist;	// Vector of 8-character strings. Tracks right history

	// Next vector helps me figure out which nodes are related to the current node. 
	vector<int> ancestors;	// Tracks the parent indices for a tree node

// Functions for editing a node
public:
	// Default constructor. All strings start as "NULL". Vectors are empty
	TreeInfo()
	{
		ID = "NULL";
		PID = "NULL";
		rawEvent = "NULL";
		lHash = "NULL";
		rHash = "NULL";
	}

	// Updates ancestor indices
	void updateAncestry(int index)
	{
		ancestors.push_back(index);
	}

	// Updates ancestor indices based on parent's ancestors
	void updateAncestry(vector<int> indices)
	{
		// vector<int> indices is the parent's set of ancestors
		for (int i = 0; i < indices.size(); i++)
		{
			ancestors.push_back(indices[i]);
		}
	}

	// Updates ID
	void updateID(string message, string parent)
	{
		string combine;	// Keep the combination of the raw event and the parent ID
		combine.append(message);	// Put raw event in the combined string
		combine.append(parent);		// Put parent ID in the combined string
		hash<string> hashIt;	// Prepare hash function from STL
		int rawHash = hashIt(combine);	// Hash the combined string
		string finalHash = to_string(rawHash);	// Convert the hash to a string
		finalHash.resize(8);	// Make sure that the hashed string is 8 chars long
		ID = finalHash;	// Update ID
	}

	// Updates PID
	void updatePID(string parent)
	{
		PID = parent;	// Update PID
		// Note: Parent ID will have to be obtained specially in main()
		//		 because the tree is being built with a vector in main()
	}

	// Updates raw event message
	void updateEvent(string message)
	{
		if (message.size() > 1024)	// Check if the message is too large
		{
			message.resize(1024);	// Resize if necessary
		}
		rawEvent = message;	// Update raw event
	}

	// Updates left hash
	void updatelHash(string lID, string lPID, string message, string lH, string rH)
	{
		string combine;	// Keep the combination of the left child's content
		combine.append(lID);		// Add in left child's ID
		combine.append(lPID);		// Add in left child's parent ID (just the current ID)
		combine.append(message);	// Add in left child's raw event
		if (lH != "NULL")
		{
			combine.append(lH);	// Add in left child's left hash
		}
		if (rH != "NULL")
		{
			combine.append(rH);	// Add in left child's right hash
		}
		hash<string> hashIt;	// Prepare hash function from STL
		int rawHash = hashIt(combine);	// Hash the combined string
		string finalHash = to_string(rawHash);	// Convert the hash to a string
		finalHash.resize(8);	// Make sure that the hashed string is 8 chars long
		lHash = finalHash;	// Update left hash
	}

	// Updates right hash
	void updaterHash(string rID, string rPID, string message, string lH, string rH)
	{
		string combine;	// Keep the combination of the left child's content
		combine.append(rID);		// Add in right child's ID
		combine.append(rPID);		// Add in right child's parent ID (just the current ID)
		combine.append(message);	// Add in right child's raw event
		if (lH != "NULL")
		{
			combine.append(lH);	// Add in right child's left hash
		}
		if (rH != "NULL")
		{
			combine.append(rH);	// Add in right child's right hash
		}
		hash<string> hashIt;	// Prepare hash function from STL
		int rawHash = hashIt(combine);	// Hash the combined string
		string finalHash = to_string(rawHash);	// Convert the hash to a string
		finalHash.resize(8);	// Make sure that the hashed string is 8 chars long
		rHash = finalHash;	// Update right hash
	}

	// Updates left history
	void updatelHist(string lH)
	{
		lHist.push_back(lH);	// Add left hash to vector
	}

	// Updates right history
	void updaterHist(string rH)
	{
		rHist.push_back(rH);	// Add right hash to vector
	}

	// Returns the ancestors of the tree element
	vector<int> getAncestry()
	{
		return ancestors;
	}

	// Returns the ID of the tree element
	string getID()
	{
		return ID;
	}

	// Returns the parent ID of the tree element
	string getPID()
	{
		return PID;
	}

	// Returns raw event of the tree element
	string getEvent()
	{
		return rawEvent;
	}

	// Returns the left hash of the tree element
	string getlHash()
	{
		return lHash;
	}

	// Returns the right hash of the tree element
	string getrHash()
	{
		return rHash;
	}

	// Prints out the information in a tree node
	void showRecords()
	{
		cout << "ID         : " << ID << endl;
		cout << "Parent ID  : " << PID << endl;
		cout << "Event      : " << rawEvent << endl;
		cout << "Left Hash  : " << lHash << endl;
		cout << "Right Hash : " << rHash << endl;
		cout << "L. History : ";
		if (lHist.empty())
		{
			cout << "No history" << endl;
		}
		else
		{
			cout << "[ ";
			for (int i = 0; i < lHist.size() - 1; i++)
			{
				cout << lHist[i] << ", ";
			}
			cout << lHist[lHist.size() - 1] << " ]" << endl;
		}
		cout << "R. History : ";
		if (rHist.empty())
		{
			cout << "No history" << endl;
		}
		else
		{
			cout << "[ ";
			for (int i = 0; i < rHist.size() - 1; i++)
			{
				cout << rHist[i] << ", ";
			}
			cout << rHist[rHist.size() - 1] << " ]" << endl;
		}
	}
};

int main()
{
	vector<TreeInfo> Ttree;	// Holds information for each node of tracking tree
	string command;	// Holds the user's input for a command
	bool loop = true;	// Determines if the main loop should close. Variable changed only once.
	int treeindex = 0;	// Tracks the latest index of Ttree. Needed to update parent indices upon insert.

	// Introduction
	cout << "Welcome to this tracking tree application!" << endl;
	cout << "Given a command, I will execute the proper" << endl;
	cout << "operation on a tracking tree." << endl << endl;
	cout << "===== Valid commands =====" << endl;
	cout << "insert   : Add information to the tree" << endl;
	cout << "showtree : Show IDs in a tree format" << endl;
	cout << "records  : Show the full record of a given ID" << endl;
	cout << "change   : Update the data associated with a given ID" << endl;
	cout << "exit     : Leave the application" << endl;
	cout << "==========================" << endl << endl;

	// Execution body
	while (loop == true)
	{
		cout << "Enter a command: ";
		cin >> command;
		cout << endl;

		if (command == "exit")
		{
			loop = false;
		}

		else if (command == "insert")
		{
			string message;	// Holds the user's input for raw event
			cout << "Enter data to be stored: ";	// Prompt for a message to be stored
			cin.ignore();
			getline(cin, message);	// Get message
			cout << endl;
			TreeInfo temp;	// Set up a temporary node to push into Ttree
			if (treeindex != 0)	// Check if we're on the first entry
			{
				int parentindex;
				if (treeindex % 2 == 1) // Check if current index is odd
				{
					// Ttree is built using levelorder structure. I observed that the index of 
					// a parent node can be found using integer division.
					// If the current index is odd, parent index == current index / 2
					// If the current index is even, parent index == (current index / 2) - 1
					// These calculations only work because of the levelorder structure.

					parentindex = treeindex / 2;	// Find the index of the direct parent
					temp.updateAncestry(parentindex);	// Add the direct parent's index to the ancestry
					temp.updateAncestry(Ttree[parentindex].getAncestry());	// Add the parent's ancestry to current ancestry
				}
				else
				{
					parentindex = (treeindex / 2) - 1;
					temp.updateAncestry(parentindex);
					temp.updateAncestry(Ttree[parentindex].getAncestry());
				}
				temp.updateEvent(message);	// Update raw event
				temp.updatePID(Ttree[parentindex].getID());	// Update parent ID by utilizing parent's index in Ttree
				temp.updateID(temp.getEvent(), temp.getPID());	// Update ID

				// If the ID created for temp is already in the tree, changing raw events and cascading histories will not work later.
				// Thus, I need a preventative measure. Simply put, I will only insert temp in the tree if its ID does not find a match.
				bool IDinTree = false;
				for (int i = 0; i < Ttree.size(); i++)
				{
					if (temp.getID() == Ttree[i].getID())
					{
						IDinTree = true;
					}
				}
				if (IDinTree != true)
				{
					Ttree.push_back(temp);	// All information gathered. Add temp to Ttree

					// At this point, we know new info has been gathered; thus, hashes and hash histories must be updated.
					// I use the ancestry vector here to go directly to the nodes that require updates.
					vector<int> parentage = Ttree[treeindex].getAncestry();	// This just makes the next for-loop easier for me to think about.
					int curridx = treeindex;	// I need this to keep track of which child I'm looking at.
					for (int i = 0; i < parentage.size(); i++)
					{
						// The following assignments make it easier for me to think about what items I must hash.
						int parentidx = parentage[i];
						string currID = Ttree[curridx].getID();
						string currPID = Ttree[curridx].getPID();
						string currEvent = Ttree[curridx].getEvent();
						string lHash = Ttree[curridx].getlHash();
						string rHash = Ttree[curridx].getrHash();

						if (curridx % 2 == 1)	// If the current index is odd, I'm looking at the parent's left child.
						{
							Ttree[parentidx].updatelHash(currID, currPID, currEvent, lHash, rHash);	// Update current parent's left hash
							Ttree[parentidx].updatelHist(Ttree[parentidx].getlHash());	// Add new left hash to left history
						}
						else
						{
							Ttree[parentidx].updaterHash(currID, currPID, currEvent, lHash, rHash);	// Update current parent's right hash
							Ttree[parentidx].updaterHist(Ttree[parentidx].getrHash());	// Add new right hash to right history
						}
						curridx = parentidx;	// At this point, the parent being updated becomes the next child to evaluate.
					}

					treeindex++;	// Increment the index tracker. In this way, I know which index of the tree I'm working with.
				}
			}
			else
			{
				temp.updateEvent(message);	// Update raw event
				string random = "random";	// Create some odd string to hash for parent ID
				hash<string> ranHash;	// Prepared hash function from STL
				string hashed = to_string(ranHash(random));	// Hash that string, convert it from int to string
				temp.updatePID(hashed);	// Update parent ID
				temp.updateID(temp.getEvent(), temp.getPID());	// Update ID
				Ttree.push_back(temp);	// All information gathered. Add temp to Ttree
				treeindex++; // We are no longer on the first entry. Inserts must now utilize existing parent IDs.
			}
		}

		else if (command == "showtree")
		{
			if (Ttree.empty())
			{
				cout << "You haven't inserted anything yet.";
			}
			else
			{
				int level = 0;	// Track which level we're on
				int entries = 0;	// Track how many entries have been printed out for a level
				for (int i = 0; i < Ttree.size(); i++)
				{
					if (entries == pow(2, level))	// Check if the maximum number of entries per level has been reached
					{
						cout << endl;	// Move to the next line
						level++;	// Increment the level
						entries = 0;	// Reset number of entries
					}
					cout << Ttree[i].getID() << "\t";	// Print out the ID of the current entry
					entries++;	// Increment the count of entries

					// This doesn't print out a perfect tree, but it does print everything in the
					// order you would read it using a levelorder structure.
				}
			}
			cout << endl << endl;
		}

		else if (command == "records")
		{
			if (Ttree.empty())
			{
				cout << "You haven't inserted anything yet." << endl;
			}
			else 
			{
				string lookup;	// Store ID user wishes to look up
				bool found = false;	// Use this to determine if user's ID is in the tree
				int foundidx;	// Use this to track the index of the user's ID (if found)
				cout << "Enter an ID to look up: ";	// Prompt for an ID
				cin >> lookup;	// Get ID
				cout << endl;
				for (int i = 0; i < Ttree.size(); i++)
				{
					if (Ttree[i].getID() == lookup)	// Check to see if the desired ID is in the tree
					{
						found = true;	// True means ID is in the tree
						foundidx = i;	// Record the index of the ID
					}
				}
				if (found == true)	// Check if the ID was found
				{
					Ttree[foundidx].showRecords();	// Display ID's data
				}
				else
				{
					cout << "Sorry. ID: " << lookup << " was not found." << endl;
				}
			}
			cout << endl;
		}

		else if (command == "change")
		{
			if (Ttree.empty())
			{
				cout << "You haven't inserted anything yet." << endl;
			}
			else
			{
				string lookup;	// Store ID user wishes to look up
				bool found = false;	// Use this to determine if user's ID is in the tree
				int foundidx;	// Use this to track the index of the user's ID (if found)
				cout << "Enter an ID to look up: ";	// Prompt for an ID
				cin >> lookup;	// Get ID
				cout << endl;
				for (int i = 0; i < Ttree.size(); i++)
				{
					if (Ttree[i].getID() == lookup)	// Check to see if the desired ID is in the tree
					{
						found = true;	// True means ID is in the tree
						foundidx = i;	// Record the index of the ID
					}
				}
				if (found == true)	// Check if the ID was found
				{
					string remessage;	// Store new message
					cout << "Enter replacement data: ";	// Prompt for new message
					cin.ignore();
					getline(cin, remessage);	// Get new message
					Ttree[foundidx].updateEvent(remessage);	// Update correct node with new raw event

					// Since information in the tree has changed, all ancestors of the changed node need updating.
					// Once again, I use the ancestry vector to update hashes and hash histories.

					vector<int> parentage = Ttree[foundidx].getAncestry();	// This just makes the next for-loop easier for me to think about.
					int curridx = foundidx;	// I need this to keep track of which child I'm looking at.
					for (int i = 0; i < parentage.size(); i++)
					{
						// The following assignments make it easier for me to think about what items I must hash.
						int parentidx = parentage[i];
						string currID = Ttree[curridx].getID();
						string currPID = Ttree[curridx].getPID();
						string currEvent = Ttree[curridx].getEvent();
						string lHash = Ttree[curridx].getlHash();
						string rHash = Ttree[curridx].getrHash();

						if (curridx % 2 == 1)	// If the current index is odd, I'm looking at the parent's left child.
						{
							Ttree[parentidx].updatelHash(currID, currPID, currEvent, lHash, rHash);	// Update current parent's left hash
							Ttree[parentidx].updatelHist(Ttree[parentidx].getlHash());	// Add new left hash to left history
						}
						else
						{
							Ttree[parentidx].updaterHash(currID, currPID, currEvent, lHash, rHash);	// Update current parent's right hash
							Ttree[parentidx].updaterHist(Ttree[parentidx].getrHash());	// Add new right hash to right history
						}
						curridx = parentidx;	// At this point, the parent being updated becomes the next child to evaluate.
					}
				}
				else
				{
					cout << "Sorry. ID: " << lookup << " was not found." << endl;
				}
			}
			cout << endl;
		}

		else
		{
			cout << "Invalid command: " << command << endl << endl;
		}
	}


    return 0;
}

