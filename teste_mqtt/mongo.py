import pymongo
import mosquitto

# Connect to the MongoDB database
client = pymongo.MongoClient("mongodb://localhost:27017/")
db = client["mydatabase"]
collection = db["mycollection"]

# Retrieve the data from the collection
data = collection.find().sort("timestamp", pymongo.DESCENDING).limit(1)

# Connect to the Mosquitto broker
mqtt_client = mosquitto.Client()
mqtt_client.connect("192.168.100.249", 1883)

# Publish the data to the Mosquitto broker
mqtt_client.publish("/", data)

# Disconnect from the Mosquitto broker
mqtt_client.disconnect()