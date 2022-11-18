import paho.mqtt.client as mqttClient
import time
import random

# Define the on_connect callback function for the connection
def on_connect(client, userdata, flags, rc):
    if rc == 0:
 
        print("Connected to broker")
 
        global Connected                        #Use global variable
        Connected = True                        #Signal connection 
 
    else:
 
        print("Connection failed")

Connected = False                               # Connection Flag

broker_address= "192.168.1.122"                 # Broker Address
port = 1883                                     # Adress Port
#user = "admin"                                  # Broker username
#password = "admin"                              # Broker Password

client = mqttClient.Client("Python")            # We will pass as argument of the constructor a unique client identifier in format of a string
#client.username_pw_set(user, password=password) # Specify a username and a password (called before the connect)
client.on_connect= on_connect                   # Callback function - alled when the broker responds to the connection request (code defined later)
client.connect(broker_address, port=port)       # Connect method - establish the connection to the broker

# Start the loop
client.loop_start()                             # run a thread in background to handle the network connection and sending/receiving data

# Since establishing the connection may take a while, we will do a loop until the previously declared Connected variable is set to true.
while Connected != True:                        #Wait for connection
    time.sleep(0.1)

# Continuous loop in a try except block that will generate a random number and publish it on the broker
try:
    while True:
 
        #value = raw_input('Enter the message:')# Function to get an input string from the command line
        value = random.randint(0,40)            # generate a random integer number
        print('Random number: {0}\r'.format(value))   # Print the random number just to visualization
        client.publish("python/teste",value)     # Publish in a topic - passing as first argument the topic where we want to publish, and has second argument the message to be sent.
        time.sleep(5)
 
# If there is a keyboard interrupt (ctrl+C) the code stops - END the loop and the connection to the broker
except KeyboardInterrupt:
 
    client.disconnect()
    client.loop_stop()