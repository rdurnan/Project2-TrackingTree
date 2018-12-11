Ryan Durnan Project2 Submission

This project creates a user interface that stores information in a tracking tree based on 
a message that the user inputs. Each node of the tree contains a unique ID used to find 
one of the user's messages, a unique parent ID that links a previous entry in the tree, one 
of the user's messages, hash values for the contents of the node's children, and vectors for 
previous hashes.

The user is given 5 commands to choose from: insert, showtree, records, change, exit. Insert 
allows the user to put another message in the tree. Showtree displays all of the IDs currently 
in the tree. Records displays information based on a given ID. Change allows the user to replace 
the message in a node based on a given ID. Exit simply closes the program. If the user inputs
an incorrect command or mistypes an ID, they are brought back to the initial command prompt. 
From there, they must input their command and ID again.
