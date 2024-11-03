from flask import Flask, request, jsonify
import mysql.connector
from datetime import datetime

app = Flask(__name__)

# Connect to MySQL
db_connection = mysql.connector.connect(
    host="localhost",
    user="your_mysql_user",
    password="your_mysql_password",
    database="sensor_data_db"
)
cursor = db_connection.cursor()

# Create table if it doesn't exist
cursor.execute('''
CREATE TABLE IF NOT EXISTS sensor_data (
    id INT AUTO_INCREMENT PRIMARY KEY,
    timestamp DATETIME NOT NULL,
    temperature FLOAT,
    humidity FLOAT,
    heat_index FLOAT,
    sound_level INT,
    mq2_ppm FLOAT,
    mq3_ppm FLOAT,
    mq135_ppm FLOAT,
    dust_density FLOAT
)
''')
db_connection.commit()

# Endpoint to receive data
@app.route('/api/sensor_data', methods=['POST'])
def receive_sensor_data():
    data = request.json
    timestamp = datetime.now()

    # Prepare SQL query
    query = '''
    INSERT INTO sensor_data (timestamp, temperature, humidity, heat_index, noise, CO2, methane, SO2, dust)
    VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s)
    '''
    values = (
        timestamp, data['temperature'], data['humidity'], data['heatIndex'],
        data['noise'], data['mq2_ppm'], data['mq3_ppm'], data['mq135_ppm'], data['dustDensityStr']
    )

    cursor.execute(query, values)
    db_connection.commit()

    return jsonify({"message": "Data received successfully"}), 200

# Run the server
if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)

