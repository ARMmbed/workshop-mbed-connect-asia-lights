# mbed Connect 2016 - Building an internet connected lighting system - Instructions

Welcome to our session at mbed Connect 2016! If you have any questions, please just give a shout. We are here to help.

In this session we'll be building four examples together, introducing you to:

* Connecting devices to mbed Device Connector.
* Interacting with devices through the mbed Device Connector API.
* Building a web app to control devices.

We will demonstrate everything on stage, but in case you're stuck this document will help you get back on track. We've also listed some additional excercises if you want a challenge.

> If you happen to be much faster than everyone else, help your neighbors.

## Prerequisites

We need to install a few pieces of software that we'll be using.

On your computer:

1. Install a recent version of [node.js](https://nodejs.org) (4.x or higher).
1. Download the source code for this workshop from [here](https://github.com/ARMmbed/workshop-mbed-connect-asia-lights) - and unpack in a convenient location.

If you are on Windows, also install:

1. [ST Link](http://janjongboom.com/downloads/st-link.zip) - serial driver for the board.
    * Run `dpinst_amd64` on 64-bits Windows, `dpinst_x86` on 32-bits Windows.
    * Afterwards, unplug your board and plug it back in.
    * (Not sure if it configured correctly? Look in 'Device Manager > Ports (COM & LPT)', should list as STLink Virtual COM Port.
1. [Tera term](https://osdn.net/projects/ttssh2/downloads/66361/teraterm-4.92.exe/) - to see debug messages from the board.

## Setup

1. Attach the Grove base shield to your development board.
1. Attach the following components to the Grove shield:
    * LED to Grove `D6` (to INPUT port on LED).
    * Accelerometer to any Grove `I2C` port.
1. Connect the K64F board to your computer.
1. Plug in an Ethernet cable to the K64F board.
1. The board mounts as a mass-storage device (like a USB drive). Verify that you can see it (the drive name will be MBED).
1. Double-click on `mbed.htm` - you'll be redirected to the [K64F platform page](https://developer.mbed.org/platforms/FRDM-K64F/).
    * If prompted to sign in, sign in or create an account.
1. Click the **Add to your mbed Compiler** button.
1. Click the **Compiler** button.
1. An IDE should open. Congratulations!

**Local development:** If you like things locally, you can do so by using [mbed CLI](https://docs.mbed.com/docs/mbed-os-handbook/en/5.1/getting_started/blinky_cli/#installing-mbed-cli-and-a-toolchain). I very much recommend to just use the online IDE, as it makes it easier for us, but if you want to continue hacking in the future, this is a nice way.

## 1. Light that responds to motion

We'll run a program that will turn the LED on whenever motion is detected.

1. Make sure that you have the online compiler open.
1. Click the Import button, then click **Click Here to import from URL**.
1. Paste the following URL: https://github.com/ARMmbed/workshop-mbed-connect-asia-lights
    * Do **NOT** tick the 'Update libraries' checkbox.
1. Click the **Import** button.
1. In the top right corner, verify that the right development board is selected (FRDM-K64F).

Next, we will select which program we will build. This step needs to be repeated every time we progress to the next example.

1. In the tree, locate 'select_project.h'.
1. Change the number in this file to the project you want to build. In this case `1`.

Now we can implement the code to make the light respond to movement. Open ``1_movement/main.h`` and under 'YOUR CODE HERE' add the following code:

```cpp
void onPirTimeout() {
  rgbLed.setColorRGB(0, 0x0, 0x0, 0x0);
}

void pir_rise() {
  // Set the color to green
  rgbLed.setColorRGB(0, 0x00, 0xff, 0x00);

  // Turn the lights off again after X seconds
  pirTimeout.attach(&onPirTimeout, 5);
}
```


1. Now press Compile.
1. A file downloads.
1. Drag the file to the 'MBED' disk.
1. The LED should turn red.
1. Whenever movement is detected the LED turns green for 5 seconds.

**Optional:** We have access to the built-in button (create an interrupt for `BLE_BUTTON_PIN_NAME`). Make it so that when you press the button the LED will go on indefinitely, ignoring the PIR sensor. Press again to listen to the PIR sensor again. This way we can simulate a normal light switch.

## 2. Connecting to mbed Device Connector

We can now bring this program into mbed Device Connector.

First we need to obtain a security certificate:

1. Go to [connector.mbed.com](https://connector.mbed.com) and sign in with your mbed credentials.
1. Click on 'Security credentials'.
1. Click on **Get my device security credentials**.
1. Copy the content of the gray box.
1. Create a new file ``security.h`` in the root folder of the project in the online compiler, and paste.

Now change the project and write some code:

1. In the tree, locate 'select_project.h' and change the number in this file to `2`.
1. This program is a bit bigger than the previous one, it does the following:
    * Has a variable (`ledStatus`) to listen to PIR (`NONE`), be permanently on (`ON`) or permanently off (`OFF`).
    * Has a variable (`ledColor`) for the color.
    * Has a variable (`ledTimeout`) for the timeout after a PIR signal.
    * Plus a variable (`pirCount`) to count how often the PIR sensor was triggered.
1. There is also code to connect to mbed Device Connector (see `main` function).
1. You can run this program fine without an internet connection, but we can turn the variables into 'cloud variables' managed by Device Connector.
1. Under `YOUR CODE HERE` paste:

```cpp
// We encode color in 3 bytes [R, G, B] and put it in an Int (default color: green)
SimpleResourceInt ledColor = client.define_resource("led/0/color", 0x00ff00, &colorChanged);
SimpleResourceInt ledTimeout = client.define_resource("led/0/timeout", 5);
SimpleResourceInt ledStatus = client.define_resource("led/0/permanent_status", STATUS_NONE, &statusChanged);
SimpleResourceInt pirCount = client.define_resource("pir/0/count", 0, M2MBase::GET_ALLOWED);
```

1. Compile and run this program.
1. When connection to the internet succeeded the built-in LED will burn.
1. LED should still go on when moving in front of the sensor.

Your device should now show as registered on: [Connected devices in mbed Device Connector](https://connector.mbed.com/#endpoints) (type: light-system).

### Showing debug messages

To show debug messages we need a serial monitor. Follow the instructions below. Your output should be similar to:

```
Hello world
[EasyConnect] Using Ethernet
[EasyConnect] Connected to Network successfully
[EasyConnect] IP address 192.168.1.16
[SMC] Device name 2bffcc03-05a6-4921-b345-ebddb52f6f71
[SMC] Registered object successfully!
```

#### Windows

To see debug messages, install:

1. [ST Link](http://janjongboom.com/downloads/st-link.zip) - serial driver for the board.
    * See above for more instructions.
1. [Tera term](https://osdn.net/projects/ttssh2/downloads/66361/teraterm-4.92.exe/) - to see debug messages from the board.

When you open Tera Term, select *Serial*, and then select the STLink COM Port.

![Tera Term](img/tera1.png)

#### OS/X

No need to install a driver. Open a terminal and run:

```
screen /dev/tty.usbm            # now press TAB to autocomplete and then ENTER
```

To exit, press: `CTRL+A` then `CTRL+\` then press `y`.

#### Linux

If it's not installed, install GNU screen (`sudo apt-get install screen`). Then open a terminal and find out the handler for your device:

```
$ ls /dev/ttyACM*
/dev/ttyACM0
```

Then connect to the board using screen:

```
sudo screen /dev/ttyACM0 9600                # might not need sudo if set up lsusb rules properly
```

To exit, press `CTRL+A` then type `:quit`.

## 3. Interacting with the light

We can now control the device from the mbed Device Connector API Console. In the previous section we created four cloud variables:

* `led/0/permanent_status` - Status of the LED. 0 = Listen to PIR, 1 = On, 2 = Off.
* `led/0/timeout` - Time after which the LED should turn off after being triggered by PIR sensor.
* `led/0/color` - Color of the LED (expressed as integer).
* `pir/0/count` - Number of times PIR sensor was triggered. When this number updates we know there was movement...

We can interact with these variables through the [API Console](https://connector.mbed.com/#console).

1. In the API Console select 'Endpoint directory lookup'.
1. Select 'GET /endpoints/{endpoint-name}/{resource-path}'.
1. Under 'endpoint' select your device.
1. Under 'resource-path' select '/pir/0/count'.
1. Click **TEST API**.
1. Move your hand in front of the PIR sensor.
1. Click **TEST API** again.
1. Verify that the number changed.

We can also write new values to the device.

1. Select the 'PUT' tab.
1. Under 'resource-path' select '/led/0/permanent_status'.
1. Under the *PUT Data tab* put '1'.
1. Click **TEST API**.
1. Verify that the LED is now on.
1. Change back to 'Parameters'.
1. Under 'resource-path' select '/led/0/color'.
1. Under 'PUT Data' put '16711935' (0xff00ff => 16711935 => pink).
1. Click **TEST API**.
1. Verify that the light is pink.
1. Change 'permanent_status' back to '0' (PIR).

## 4. From code

We can also write some code to interact with this device.

1. Install a recent version of [node.js](https://nodejs.org) (4.x or higher).
1. Download the source code for this workshop from [here](https://github.com/ARMmbed/workshop-mbed-connect-asia-lights/archive/master.zip) - and unpack in a convenient location.
1. Open a terminal or a command window.
1. Change directory to the folder where you download the `workshop-mbed-connect-asia-lights` repository.
1. Run `cd 4_from_code`.
1. Run `npm install`.
1. Open `main.js` in a text editor.

Now we need an access key (API key).

1. Go to the [Access Keys](https://connector.mbed.com/#accesskeys) page.
2. Click **Generate new key**
3. Copy the key, and paste it in `main.js` on the first line.
4. Go back to the terminal.
5. Run `node main.js`.

Output should be something like:

```
Found 1 lights [ { name: '2bffcc03-05a6-4921-b345-ebddb52f6f71',
    type: 'light-system',
    status: 'ACTIVE' } ]
```

Now we can subscribe to movement... Under 'YOUR CODE HERE' add:

```js
    api.putResourceSubscription(endpoint, '/pir/0/count', function(err) {
      console.log('Set subscription for pir count', err);
    });
```

1. Restart the program (from the terminal, press CTRL+C and start again).
1. Move your hand in front of the sensor.
1. Output should show in the terminal!

We can also control the device... Under 'YOUR CODE HERE' add:

```js
    api.putResourceValue(endpoint, '/led/0/permanent_status', 1, function(err) {
      if (err) return console.error('Failed to set status...', err);

      console.log('Set status to 1...');

      api.putResourceValue(endpoint, '/led/0/color', 0xffa500, function(err) {
        if (err) return console.error('Failed to set color...', err);

        console.log('Set color to orange!');
      });
    });
```

And run again.

## 5. An app

We use a web app to control all our lights now...

1. In your terminal navigate to the `5_an_app` folder.
1. Run `npm install`.
1. Open `main.js` and paste your access key in again.
1. Run `node main.js`.
1. Output should be something like:

```
connected to mbed Cloud, retrieving initial device model
got devices [ { name: '2bffcc03-05a6-4921-b345-ebddb52f6f71',
    type: 'light-system',
    status: 'ACTIVE',
    endpoint: '2bffcc03-05a6-4921-b345-ebddb52f6f71' } ]
subscribed to 2bffcc03-05a6-4921-b345-ebddb52f6f71 /pir/0/count
got value 2bffcc03-05a6-4921-b345-ebddb52f6f71 status /led/0/permanent_status = 1
got value 2bffcc03-05a6-4921-b345-ebddb52f6f71 timeout /led/0/timeout = 5
got value 2bffcc03-05a6-4921-b345-ebddb52f6f71 color /led/0/color = 16753920
en0 192.168.1.11
Web server listening on port 5265!
```

1. Go to http://localhost:5265 and you can now interact with your devices.
    * Color picker does not work in Safari... Try Firefox or Chrome.
1. We can also show notifications whenever movement happens... Open `5_an_app/views/index.html` and add under 'YOUR CODE HERE':

```js
    // Movement detected! This is an event sent by the server...
    socket.on('change-count', function(endpoint, count) {
      // At the bottom of the page show a notification :-)
      showNotification('Movement detected at ' + endpoint);
    });
```

*(refresh the page for this to take effect)*

### On your mobile phone

You can also see the page on your mobile phone.

1. Look in the node.js output when you started 5_an_app (in the terminal).
1. Find a line similar to `en0 192.168.1.11`, that's your IP address.
1. Connect your phone to the same WiFi network as your **laptop** (not the same network as your device).
1. On your phone go to http://YOUR_IP:5265 (like http://192.168.1.11:5625).
1. See the same control app.
1. If you change something on the phone it also changes on your laptop!
1. On Chrome for Mobile? Click 'Add to homescreen' and it will look just like a real app =).
