## Key Points
To insert data from your Arduino to the MySQL database, you can set up an ESP32 (or similar Wi-Fi-capable microcontroller) to act as a bridge between the Arduino and the MySQL server. This setup enables the ESP32 to receive sensor data from the Arduino, format it into SQL commands, and send those commands to the MySQL database via an HTTP request to a server-side script, like flask, which then interacts with the database.


#### Requirements
You need 
- Arduino IDE [download here](https://www.arduino.cc/en/software)
- Python, [download here](https://www.python.org/downloads/)

  
Clone this project and open the two folders contain the sketch in Arduino IDE

Upload `comp_DustSensor_Sensordata.ino` to an `Arduino UNO`

Upload `EPS32_Send_Sensory_Data_to_Server.ino` to `Arduino Nano EPS32` [read more about Arduino Nano EPS32 here](https://docs.arduino.cc/hardware/nano-esp32/)

if you don't know how to upload a sketch to an Arduino board, check more here [https://wiki-content.arduino.cc/en/Guide/ArduinoUno](https://wiki-content.arduino.cc/en/Guide/ArduinoUno)

After uploading the sketch to the two board, you connect them. [connect Arduino UNO to Arduino Nano esp32](https://youtu.be/_A2KwuiE7Ww)

On another laptop run the `Server` folder

#### WiFi Configuration on ESP32: 
- Replace `your_wifi_ssid` and `your_wifi_password` with your WiFi credentials.
- Set the `<your-server-url>` variable to your server endpoint where data will be received.

#### The Server 
The server-side code was built with `python` using `flask`. Ensure you've installed `Python version 3.x.x or above`

It has a single post route `/api/sensor_data` that insert the sensory data received from the `EPS32 (Wi-Fi-capable microcontroller)` to `MySQL database`, which your will create on your `MySQL Server`. Use `sensor_data_db` for the database name.

- Config your `MySql server` on your machine and enter it database connection settings in `./server/app.py`
- Then run server by running
  
  ```bash
  cd server
  ```
  ```bash
  pip install -r requirements.txt
  ```
  ```bash
  python app.py
  ```
- Get the server local ip address e.g `https://192.23.34.23:5000` and replace it to `<your-server-url>`

#### More Information
Arduino get the sensory data,
the ESP32 reads data sent from the Arduino, then forwards it to the server using an HTTP POST request where it is stored in a `MySQL Database`.

Note: This setup allows the MCU to read sensor data, send it to the ESP32 over Serial, and finally, the ESP32 forwards it to a server  where it is stored in a `MySQL Database`.
Read more here [https://arduinogetstarted.com/tutorials/arduino-temperature-humidity-sensor](https://arduinogetstarted.com/tutorials/arduino-temperature-humidity-sensor)
