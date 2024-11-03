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
    temperature TEXT,
    humidity TEXT,
    heat_index TEXT,
    noise TEXT,
    CO2 TEXT,
    methane TEXT,
    SO2 TEXT,
    dust TEXT
)
''')
db_connection.commit()

# Endpoint to receive data
@app.route('/api/sensor_data', methods=['POST','GET'])
def receive_sensor_data():
    if request.method == 'POST':
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
    elif request.method == 'GET':
        # Prepare SQL query for retrieving data
        query = 'SELECT * FROM sensor_data ORDER BY timestamp DESC'
        cursor.execute(query)
        results = cursor.fetchall()

        # Format the results for JSON response
        data = []
        for row in results:
            data.append({
                "timestamp": row[0],
                "temperature": row[1],
                "humidity": row[2],
                "heat_index": row[3],
                "noise": row[4],
                "CO2": row[5],
                "methane": row[6],
                "SO2": row[7],
                "dust": row[8]
            })
        return jsonify(data), 200

# Run the server
if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)

