TreeRePair is a prototype implementation of the Re-pair for Trees algorithm which compresses the tree structure of XML documents. 

For more information see:
Markus Lohrey, Sebastian Maneth, Roy Mennicke: Tree Structure Compression with RePair. DCC 2011:353-362

I am not the original author of the code. This is merely a clone of the original code from <https://code.google.com/archive/p/treerepair/>

Compiling the source code
=========================

Execute

make -f Makefile.static 

to create the TreeRePair and TreeDePair executables (boost library is needed).


Executing TreeRePair
====================

- cd into TreeRePair folder
- execute ./TreeRePair


Executing TreeDePair (decompressor)
===================================

- cd into TreeDePair folder
- execute ./TreeDePair
