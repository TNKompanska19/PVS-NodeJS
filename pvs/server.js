// server.js
const express = require('express');
const mysql = require('mysql');
const cors = require('cors');

const app = express();
const port = 3000;

app.use(cors());

const connection = mysql.createConnection({
  host: 'localhost',
  user: 'root',
  password: '', // Add your MySQL password
  database: 'sensors',
});

connection.connect();

app.get('/sensors-data', (req, res) => {
  const query = `
    SELECT 
      temperature as Temperature,
      humidity as Humidity,
      light as Light,
      DATE(timestamp) as Date
      FROM meteorological_station
  `;

  connection.query(query, (error, results) => {
    if (error) {
      console.error('Error fetching data:', error);
      res.status(500).json({ error: 'Internal Server Error' });
    } else {
      const humidityData = results.map(result => ({
        Temperature: result.Temperature,
        Humidity: result.Humidity,
        Light: result.Light,
        Rain: result.Rain,
        Date: new Date(result.Date).toLocaleDateString()
      }));
      res.json({ data: humidityData });
    }
  });
});

app.listen(port, () => {
  console.log(`Server listening at http://localhost:${port}`);
});
