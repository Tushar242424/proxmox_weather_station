const express = require('express');
const bodyParser = require('body-parser');
const ejs = require('ejs');

const app = express();
const port = 3001;

// Set up EJS as the view engine
app.set('view engine', 'ejs');

// Parse JSON and URL-encoded bodies
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

// Middleware to log incoming requests
app.use((req, res, next) => {
  console.log(`Received ${req.method} request at ${req.url}`);
  next();
});


app.use(express.static('public'));
// Default values
let data = {
  humidity: 0,
  temperature: 0,
  airPressure: 0,
  isRainy: false,
  isSunny: false,
};

// Render the index page with the data
app.get('/', (req, res) => {
  res.render('index', { data });
});

// Handle the PUT request
app.put('/updateData', (req, res) => {
  // Update data with values from the request body
  data = {
    humidity: req.body.humidity || 0,
    temperature: req.body.temperature || 0,
    airPressure: req.body.airPressure || 0,
    isRainy: req.body.isRainy || false,
    isSunny: req.body.isSunny || false,
  };

  // Log the updated data
  console.log('Updated data:', data);

  // Send a response
  res.send('Data updated successfully');
});

// Start the server
app.listen(port, () => {
  console.log(`Server listening at http://localhost:${port}`);
});
