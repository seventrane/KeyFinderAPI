require('dotenv').config();

/**
 * Module dependencies.
 */

const app = require('./app');
const http = require('http');
const express = require('express');
//const cors = require('cors');
const multer = require('multer');
const fs = require('fs');
const path = require('path');
const addon = require('../build/Release/addon');

// Enable CORS for all routes
//app.use(cors());

/**
 * Get port from environment and store in Express.
 */

const port = normalizePort(process.env.PORT || '3000');
app.set('port', port);

/**
 * Create HTTP server.
 */

const server = http.createServer(app);

/**
 * Listen on provided port, on all network interfaces.
 */

server.listen(port);
server.on('error', onError);
server.on('listening', onListening);

/**
 * Normalize a port into a number, string, or false.
 */

function normalizePort(val) {
  const port = parseInt(val, 10);

  if (isNaN(port)) {
    // named pipe
    return val;
  }

  if (port >= 0) {
    // port number
    return port;
  }

  return false;
}

/**
 * Event listener for HTTP server "error" event.
 */

function onError(error) {
  if (error.syscall !== 'listen') {
    throw error;
  }

  const bind = typeof port === 'string'
    ? 'Pipe ' + port
    : 'Port ' + port;

  // handle specific listen errors with friendly messages
  switch (error.code) {
    case 'EACCES':
      console.error(bind + ' requires elevated privileges');
      process.exit(1);
      break;
    case 'EADDRINUSE':
      console.error(bind + ' is already in use');
      process.exit(1);
      break;
    default:
      throw error;
  }
}



const storage = multer.diskStorage({
  destination: 'uploads/',
  filename: function (req, file, cb) {
    // Preserve the original file name
    cb(null, file.originalname);
  }
});

const upload = multer({ storage: storage }); // Use custom storage options

// Define a route to handle HTTP POST requests for file uploads
app.post('/api/upload', upload.array('audioFiles'), (req, res) => {
  // Check if files were uploaded
  if (!req.files || req.files.length === 0) {
    return res.status(400).json({ error: 'No files uploaded!' });
  }

  // Extract file paths from the uploaded files
  const filePaths = req.files.map(file => file.path);

  // Call the addon function with each file path
  const results = filePaths.map(filePath => addon.myFunction(filePath));

  // Send the results as JSON response
  res.json({ success: true, results: results });
});

// Define a route to handle HTTP GET requests
app.get('/api/myFunction', (req, res) => {
    // Extract folderPath from request query parameters
    const folderPath = req.query.folderPath;

    // Check if folderPath is provided
    if (!folderPath) {
        return res.status(400).json({ error: 'Folder path is required' });
    }

    // Call the addon function with the folderPath
    const result = addon.myFunction(folderPath);

    // Send the result as JSON response
    res.json({ result: result });
});

/**
 * Event listener for HTTP server "listening" event.
 */

function onListening() {
  const addr = server.address();
  const bind = typeof addr === 'string'
    ? 'pipe ' + addr
    : 'port ' + addr.port;
  console.log('App started. Listening on ' + bind);
}





// Start the server
const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
  console.log(`Server is running on port ${PORT}`);
});