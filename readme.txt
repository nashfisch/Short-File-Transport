1) Raj Pannu, Nash Fischer, Kaleb Calverly

2) Create a directory with all the submitted files inside. Type "make" into the command line to generate the sender executable. Navigate into the created bin directory
   "cd /bin". Here you will find the sender executable, as well as a receiver executable that is NOT ROBERT's. 
   
   When receiving, use Robert's executable in a different directory or replace the generated one with his one. To receive a file, ensure you are in a directory with robert's
   executable, and type "receiver -f [filename]" where [filename] is the name of the file. 
   
   Ensure that on the sender terminal, if you wish to transmit a file, that the file is in the bin directory as well. To transmit a file, type 
   "sender -f [filename] [host] [port]" where [filename] is the name of the file, [host] is the address of the host (IPV4 _._._._ format for IPV4 transmission, 
   IPV6 :: format for IPV6 transmission), and [port] is the port number. You may ommit the "-f" and [filename], and input your message into the console instead.
