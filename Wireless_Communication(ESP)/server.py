# import socket			

# s = socket.socket()		
# print ("Socket successfully created")


# # the port number shud be same in esp and server
# port = 12370
			
# # server IPv4 address
# # use ipconfig cmd to find the value 
# s.bind(('192.168.104.29', port))	
# print ("socket binded to %s" %(port))

# s.listen(10)	
# print ("socket is listening")		

# while True:

#     # Establish connection with client.
#     c, addr = s.accept()	
#     print ('Got connection from', addr )
#     while True:
#         x = c.recv(1024).decode()
        # y = c.recv(1024).decode()
        # z = c.recv(1024).decode()
        # x1 = c.recv(1024).decode()
        # y1 = c.recv(1024).decode()
        # z1 = c.recv(1024).decode()
        #print(x)
        # print(y)
        # print(z)
        # print(x1)
        # print(y1)
        # print(z1)
        # write the values to csv, where every row will have x,y,z values
    




# import socket
# import csv

# s = socket.socket()
# print("Socket successfully created")

# # the port number should be same in esp and server
# port = 12370

# # server IPv4 address
# # use ipconfig cmd to find the value
# s.bind(('192.168.2.102', port))
# print("socket binded to %s" % (port))

# s.listen(10)
# print("socket is listening")

# # create a CSV file to write the values
# with open('received_values.csv', mode='w', newline='') as file:
#     writer = csv.writer(file)
#     writer.writerow(['x', 'y', 'z'])

#     while True:

#         # Establish connection with client.
#         c, addr = s.accept()
#         print('Got connection from', addr)
        
#         while True:
#             v = c.recv(1024).decode()
#             if not v:
#                 break
#             print(v)
#             x, y, z = v.split(',') # assuming the received data is comma separated
#             writer.writerow([x, y, z])




# import socket
# import csv
# from datetime import datetime

# s = socket.socket()    
# print("Socket successfully created")

# # the port number should be the same in esp and server
# port = 12370

# # server IPv4 address
# # use ipconfig cmd to find the value 
# s.bind(('192.168.104.29', port))  
# print("socket binded to %s" % port)

# s.listen(10)    
# print("socket is listening")

# output = [] 

# while True:
#     # Establish connection with client.
#     c, addr = s.accept()    
#     print('Got connection from', addr)
#     while True:
#         x = c.recv(1024).decode()
#         print(x)

#         # Add the output to a list and export it to a CSV file
#         output.append({'Message': x, 'Timestamp': datetime.now()})
#         with open('C:/Users/LEGION/Desktop/esp8266values/socket_messages.csv', 'w', newline='') as csvfile:
#             writer = csv.DictWriter(csvfile, fieldnames=['Message', 'Timestamp'])
#             writer.writeheader()
#             writer.writerows(output)

#  


import socket
import csv

# Create a socket
s = socket.socket()

# Bind the socket to a port
port = 12370
s.bind(('192.168.2.115', port))

# Listen for connections
s.listen(10)

# Create a CSV file
with open('output.csv', 'w') as f:
    writer = csv.writer(f)
    writer.writerow(['IP address', 'Data'])

# Accept connections and write the data to the CSV file
while True:
    c, addr = s.accept()
    print('Got connection from', addr)
    while True:
        x = c.recv(1024).decode()
        writer.writerow([addr[0], x])
