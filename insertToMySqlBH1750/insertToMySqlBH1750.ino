#include <Wire.h>
#include <SPI.h>
#include <MySQL_Generic.h>
#include <BH1750.h>  
BH1750 lightMeter;
// Defining BME280 sensor
#define SEALEVELPRESSURE_HPA (1013.25)


// MySQL Debug Level from 0 to 4
#define _MYSQL_LOGLEVEL_      1
#define MYSQL_DEBUG_PORT      Serial

// Network Info
char ssid[] = "OS";
char pass[] = "123456789";

// Database Address
char server[] = "192.168.0.1";
uint16_t server_port = 3306;

// Database Account
char user[] = "html";
char password[] = "password";

//Database Name
char database[] = "sensors";
char table[] = "meteorological_station";

// Default insert data values
double temperature = 0;
double humidity = 0;

// MySQL something
MySQL_Connection conn((Client *)&client);
MySQL_Query *query_mem;

void setup()
{
   
  // Setup Serial
  Serial.begin(9600);
  Wire.begin();
  // Begin WiFi section
  Serial.println("================================================");
  Serial.println(String("Connecting to ") + ssid);
  WiFi.begin(ssid, pass);

  lightMeter.begin();
  
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  // Print out info about the WiFi connection
  Serial.print("Connected to network. My IP address is: ");
  Serial.println(WiFi.localIP());

  // Test MySQL connection
  Serial.print("Connecting to SQL Server @ ");
  Serial.print(server);
  Serial.println(String(", Port = ") + server_port);
  Serial.println(String("User = ") + user + String(", PW = ") + password + String(", DB = ") + database);

}


void loop()
{

  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(1000);

  Serial.println("================================================");

  // change Insert Query with new data
  String INSERT_SQL = String("INSERT INTO ") + database + "." + table + " (light) VALUES (" + lux +  ")";

  // Insert data in to database
  Serial.println("Connecting to MySQL...");
  Serial.println();
  if (conn.connectNonBlocking(server, server_port, user, password) != RESULT_FAIL)
  {
    delay(500);
    // Initiate the query class instance
    MySQL_Query query_mem = MySQL_Query(&conn);

    if (conn.connected())
    {
      Serial.println(INSERT_SQL);

      // Execute the query
      // KH, check if valid before fetching
      if ( !query_mem.execute(INSERT_SQL.c_str()) )
        Serial.println("Insert error");
      else
        Serial.println("Data Inserted.");
    }
    else
    {
      Serial.println("Disconnected from Server. Can't insert.");
    }
    conn.close();                     // close the connection
  }
  else
  {
    Serial.println("\nConnect failed. Trying again on next iteration.");
  }

  // Turn off status LED
  digitalWrite(LED_BUILTIN, HIGH);

  // Sleep till next data measure and insert
  Serial.println("================================================");
  Serial.println("\nSleeping...");
  delay(5000);
}
