## Key Points
#### WiFi Configuration on ESP32: 
- Replace `your_wifi_ssid` and `your_wifi_password` with your WiFi credentials.
- Set the `<your-server-url>` variable to your server endpoint where data will be received.

#### Server
- Config your `MySql server` on your machine and enter it connection credentials in `./server/app.py`
- Then run server by running
  
  ```bash
  cd server
  ```
  ```bash
  python app.py
  ```
- Get the server local ip address e.g `https://192.23.34.23:5000` and replace it to `<your-server-url>`

#### More Information
Arduino get the sensory data,
the ESP32 reads data sent from the Arduino, then forwards it to the server using an HTTP POST request where it is stored in a `MySQL Database`.

Note: This setup allows the MCU to read sensor data, send it to the ESP32 over Serial, and finally, the ESP32 forwards it to a server  where it is stored in a `MySQL Database`.
