-- Create the main database
CREATE DATABASE IF NOT EXISTS environment_monitoring;
USE environment_monitoring;

-- Table for DHT22 Sensor Data
CREATE TABLE IF NOT EXISTS dht_sensor_data (
    id INT AUTO_INCREMENT PRIMARY KEY,
    humidity FLOAT NOT NULL,
    temperature FLOAT NOT NULL,
    heat_index FLOAT NOT NULL,
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Table for Sound Sensor Data
CREATE TABLE IF NOT EXISTS sound_sensor_data (
    id INT AUTO_INCREMENT PRIMARY KEY,
    sound_level INT NOT NULL,
    noise_detected BOOLEAN DEFAULT FALSE,
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Table for MQ2 Gas Sensor Data
CREATE TABLE IF NOT EXISTS mq2_sensor_data (
    id INT AUTO_INCREMENT PRIMARY KEY,
    sensor_value FLOAT NOT NULL,
    ppm FLOAT NOT NULL,
    smoke_detected BOOLEAN DEFAULT FALSE,
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Table for MQ3 Gas Sensor Data
CREATE TABLE IF NOT EXISTS mq3_sensor_data (
    id INT AUTO_INCREMENT PRIMARY KEY,
    sensor_value FLOAT NOT NULL,
    ppm FLOAT NOT NULL,
    high_alcohol BOOLEAN DEFAULT FALSE,
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Table for MQ135 Gas Sensor Data
CREATE TABLE IF NOT EXISTS mq135_sensor_data (
    id INT AUTO_INCREMENT PRIMARY KEY,
    sensor_value FLOAT NOT NULL,
    ppm FLOAT NOT NULL,
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Table for Dust Sensor Data
CREATE TABLE IF NOT EXISTS dust_sensor_data (
    id INT AUTO_INCREMENT PRIMARY KEY,
    dust_density FLOAT NOT NULL,
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- (Optional) Centralized Log Table
CREATE TABLE IF NOT EXISTS sensor_data_log (
    id INT AUTO_INCREMENT PRIMARY KEY,
    humidity FLOAT,
    temperature FLOAT,
    heat_index FLOAT,
    sound_level INT,
    noise_detected BOOLEAN,
    mq2_ppm FLOAT,
    mq2_smoke_detected BOOLEAN,
    mq3_ppm FLOAT,
    mq3_high_alcohol BOOLEAN,
    mq135_ppm FLOAT,
    dust_density FLOAT,
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
