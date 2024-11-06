from flask import Flask, request, jsonify, render_template
import mysql.connector
from flask_cors import CORS
from datetime import datetime

app = Flask(__name__)
# allow cors
CORS(app, resources={r"/*": {"origins": ["http://localhost:3000", "http://127.0.0.1:3000"]}})

# Connect to MySQL
db_connection = mysql.connector.connect(
    host="localhost",
    user="root", #your_mysql_user
    password="12345678", #your_mysql_password
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

# Template routes
@app.route('/',methods=['GET'])
def index_page():
    # Prepare SQL query for retrieving data
    query = 'SELECT * FROM sensor_data ORDER BY timestamp DESC'
    cursor.execute(query)
    results = cursor.fetchall()

    sensory_data = []
    for row in results:
        sensory_data.append({
            "id":row[0],
            "timestamp": row[1],
            "temperature": row[2],
            "humidity": row[3],
            "heat_index": row[4], # heatIndex
            "noise": row[5],
            "CO2": row[6], # mq2_ppm
            "methane": row[7], # mq2_ppm
            "SO2": row[8], # mq135_ppm
            "dust": row[9] # dustDensityStr
        })
    return render_template('index.html',sensory_data=sensory_data)

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
    app.run(debug=True)
    app.run(host='0.0.0.0', port=5000)

