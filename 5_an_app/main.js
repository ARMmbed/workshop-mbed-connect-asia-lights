var TOKEN = 'YOUR_ACCESS_TOKEN';

var konekuta = require('konekuta');
var express = require('express');
var app = express();
var server = require('http').Server(app);
var io = require('socket.io')(server);
var hbs = require('hbs');
var printIps = require('./get_ips');

// Some options for express (node.js web app library)
hbs.registerPartials(__dirname + '/views/partials');
app.use(express.static(__dirname + '/public'));
app.set('view engine', 'html');
app.set('views', __dirname + '/views');
app.engine('html', hbs.__express);

if (!process.env.TOKEN && TOKEN === 'YOUR_ACCESS_TOKEN') {
  throw 'Please set your access token first in main.js!';
}

// This is how we go from device => view model
// we'll use this to render the device view, this will also be sent to the client
// when a device connects (so it knows what to render)
function mapToView(d) {
  var hex = Number(d.color).toString(16);
  var model = {
    name: d.endpoint,
    rawColor: d.color,
    color: '#' + '000000'.substring(0, 6 - hex.length) + hex,
    motionClass: d.status == 0 ? 'selected' : '',
    onClass: d.status == 1 ? 'selected' : '',
    offClass: d.status == 2 ? 'selected' : '',
    timeout: d.timeout,
    status: d.status
  };

  // create the device HTML
  var html = hbs.handlebars.compile('{{> device}}')(model);

  return { model: model, html: html };
}

var options = {
  endpointType: 'light-system',     // what endpoint types to look for
  token: TOKEN,
  io: io,
  deviceModel: {                    // building the initial device model (w/ 4 properties)
    status: {
      retrieve: '/led/0/permanent_status',   // when device registers, retrieve value
      update: {
        method: 'put',                      // update actions
        path: '/led/0/permanent_status'
      }
    },
    timeout: {
      retrieve: '/led/0/timeout',
      update: {
        method: 'put',
        path: '/led/0/timeout'
      }
    },
    color: {
      retrieve: '/led/0/color',
      update: {
        method: 'put',
        path: '/led/0/color'
      }
    },
    count: {
      subscribe: '/pir/0/count'                       // subscribe to updates
    }
  },
  timeout: 20000,
  mapToView: mapToView,
  verbose: true                         // Verbose logging
};

// Start konekuta (connects to mbed Cloud, and retrieves initial device model)
konekuta(options, (err, devices, ee, connector) => {
  if (err) {
    console.error('Error connecting to mbed Device Connector...', err);
    return;
  }

  // Now we can start the web server
  server.listen(process.env.PORT || 5265, process.env.HOST || '0.0.0.0', function() {
    printIps();
    
    console.log('Web server listening on port %s!', process.env.PORT || 5265);
  });

  // And handle requests
  app.get('/', function(req, res, next) {
    // Render index view, with the devices based on mapToView function
    res.render('index', { devices: devices.map(d => mapToView(d).model) });
  });
});
