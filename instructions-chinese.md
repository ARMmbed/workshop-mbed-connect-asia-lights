# mbed Connect 2016 - Building an internet connected lighting system - Instructions

欢迎来到mbed Connect 2016， 在这节的讲解中，如果您有任何疑问，请随时提问，这节活动的宗旨就是解答您的任何问题，帮助您更快上手mbed。

今天，我们将把四个应用放在一起，做一个端到端的案例。

* 把终端设备连接到mbed Device Connector
* 通过mbed Device Connector 的API 来与终端设备交互
* 做一个网页应用来控制终端设备

我们将会在台上展示所有的过程，但是如果你觉得这个文档有任何不清晰的地方，请及时提出，我们可以帮助您解决疑问。如果您希望在今天试一下开发更复杂的应用，在文档的最后，我们也列出了一些附加题。

> 如果您能很快得完成每一个步骤，希望您能帮助您身边的人。

## 先决条件

我们先要安装几个软件：

1. [Node.js 版本4.0及以上](https://nodejs.org).
1. 下载今天我们要做的应用的源代码 并解压。 [here](https://github.com/ARMmbed/workshop-mbed-connect-asia-lights)

如果你用的是Windows：

1. [mbed Windows Serial port driver](https://developer.mbed.org/media/downloads/drivers/mbedWinSerial_16466.exe) - serial driver for the board.
    * (Not sure if it configured correctly? Look in 'Device Manager > Ports (COM & LPT)', should list the board here...
1. [Tera term](https://osdn.net/projects/ttssh2/downloads/66361/teraterm-4.92.exe/) - 用来查看串口.

## Setup

1. 把Grove base shield插到开发版上。
1. 把下列元件插到base shield上：
    *  把LED插到D6接口
    *  加速计插入任意一个I2C的接口
1. Connect the K64F board to your computer, use the USB port **left** from the Ethernet port (OpenSDA).
1. Plug in an Ethernet cable to the K64F board.
1. 电脑上会显示一个U盘设备叫MBED
1. U盘里面有一个文件叫mbed.htm，双击打开，你将会进入[FRDM-K64F platform page](https://developer.mbed.org/platforms/FRDM-K64F/)页面。
    * 如果要登入，请注册一个账号并登入。
1. 点击**Add to your mbed Compiler**，在页面的右边， 可能需要向下滚动一下。
1. 点击页面右上的**Compiler**按钮
1. 一个在线开发环境会打开。

**线下开发：** 如果你想试下离线开发的工具，请安装[mbed CLI](https://docs.mbed.com/docs/mbed-os-handbook/en/5.1/getting_started/blinky_cli/#installing-mbed-cli-and-a-toolchain)。线上线下的源代码都是一样的。

## 1. 移动触发的LED灯

现在我们做一个由振动触发的LED灯。

1. 打开线上开发环境
1. 按“import” 按钮来载入源代码。, then click **Click Here to import from URL**.
1. 在弹出的对话框里粘贴这个地址： https://github.com/ARMmbed/workshop-mbed-connect-asia-lights
    * 注意不要勾选 'Update libraries’ 的选项。
1. 按**Import**来载入。
1. 在右上角的开发版选择按钮中，选择FRDM-K64F。

下面，我们将选择程序来进行编译。下面的步骤我们会重复几次，以编译不同的模版程序。

1. 在左边的文件列表中，选择‘select_project.h'
1. 把文件中的数字改成你想编译的程序的数字。这里我们先选择`1`。

Now we can implement the code to make the light respond to movement. Open ``1_movement/main.h`` and under 'YOUR CODE HERE' add the following code:

下面我们把这个程序模版填充完整。

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

1. 按“Compile”来进行编译
1. 编译好的文件会自动开始下载
1. 把下载好的文件拖到 MBED 的U盘里。
1. Press 'RESET' button on the board when flashing is complete.
1. 开发板上的LED灯会变红。
1. 有震动时，LED灯会变绿。

**附加题：** 用开发板上的按钮，控制LED的开关。按一下，LED常亮，再按一下，LED灯回到由传感器触发。提示：给 `BLE_BUTTON_PIN_NAME` 写一个 interrupt 函数。


## 2. 连接到 mbed Device Connector

我们现在可以将我们的开发板连接到mbed Device Connector上了。

首先，我们要下载一个安全证书：

1. 前往[connector.mbed.com](https://connector.mbed.com)， 用你mbed的账户登陆。
1. 在左边的菜单里选择'Security credentials'.
1. 点击**Get my device security credentials**.
1. 复制灰色部分的内容。
1. 回到mbed在线开发环境的页面，在项目的根目录下创建一个叫做``security.h``的新文件，并把刚才复制的内容粘贴进去。

Now change the project and write some code:

1. 在左边的文件浏览器里，找到 'select_project.h'， 把这个文件里面的数字改成 `2`.

1. 这个程序比上一个更加复杂一些：
    * 有一些变量 (`ledStatus`) 来监听红外线传感器的值：常亮(`ON`) 或者常灭(`OFF`).
    * (`ledColor`) 来控制颜色
    * (`ledTimeout`)来控制红外传感器的超时
    * (`pirCount`)来记红外传感器被触发的次数。
1. 另外的代码是用来连接到·mbed Device Connector· 的。详见 `main` 函数。
1. 这个程序可以离线执行，但是如果你给板子连上网线，你的本地变量就会自动被复制到云端，变成云变量。
1. 在程序里面找到`YOUR CODE HERE`， 并把下面的代码粘贴进去

```cpp
// We encode color in 3 bytes [R, G, B] and put it in an Int (default color: green)
SimpleResourceInt ledColor = client.define_resource("led/0/color", 0x00ff00, &colorChanged);
SimpleResourceInt ledTimeout = client.define_resource("led/0/timeout", 5);
SimpleResourceInt ledStatus = client.define_resource("led/0/permanent_status", STATUS_NONE, &statusChanged);
SimpleResourceInt pirCount = client.define_resource("pir/0/count", 0, M2MBase::GET_ALLOWED);
```

1. 现在，跟之前一样，编译， 并把编译好的文件复制粘贴到板子上。
1. 板子在连上网之后，LED灯会开始闪烁。
1. LED在探测到移动的时候也会继续闪烁

你的板子现在应该已经在云端出现了[Connected devices in mbed Device Connector](https://connector.mbed.com/#endpoints)。 (type: light-system).

### 打开调试信息

如果你要调试程序，你需要打开串口。下面我们将教你怎么调试，你应该可以在串口上面看到如下的信息：

```
Hello world
[EasyConnect] Using Ethernet
[EasyConnect] Connected to Network successfully
[EasyConnect] IP address 192.168.1.16
[SMC] Device name 2bffcc03-05a6-4921-b345-ebddb52f6f71
[SMC] Registered object successfully!
```

#### Windows

如需要调试，请安装以下的软件：

1. [mbed Windows Serial port driver](https://developer.mbed.org/media/downloads/drivers/mbedWinSerial_16466.exe).
    * See above for more instructions.
1. [Tera term](https://osdn.net/projects/ttssh2/downloads/66361/teraterm-4.92.exe/) - 串口监听器。

打开Tera Term，选择“串口”，然后选择 COM 口。

![Tera Term](img/tera1.png)

#### OS/X

无需安装驱动，打开终端，直接执行以下命令

```
screen /dev/tty.usbm            # now press TAB to autocomplete and then ENTER
```

如果要推出，`CTRL+A` 然后 `CTRL+\` 然后 `y`.

#### Linux

如果你没有screen这个程序，安装screen (`sudo apt-get install screen`），打开终端，并找到你的开发板的路径，如下：

```
$ ls /dev/ttyACM*
/dev/ttyACM0
```

然后通过screen来连接到板子的串口：

```
sudo screen /dev/ttyACM0 9600                # might not need sudo if set up lsusb rules properly
```

如要推出，按`CTRL+A` 然后键入 `:quit`.

## 3. 人灯互动

来控制我们的LED。在前一章我们建立了四个云变量。

* `led/0/permanent_status` - Status of the LED. 0 = Listen to PIR, 1 = On, 2 = Off.
* `led/0/timeout` - Time after which the LED should turn off after being triggered by PIR sensor.
* `led/0/color` - Color of the LED (expressed as integer).
* `pir/0/count` - Number of times PIR sensor was triggered. When this number updates we know there was movement...

我们可以通过API Console](https://connector.mbed.com/#console) 来读写这几个变量。

1. 在API Console里，选择'Endpoint directory lookup’。
1. 选择'GET /endpoints/{endpoint-name}/{resource-path}’.
1. 在’endpoint’部分，选择你的板子。
1. 在’resource-path'部分，选择 '/pir/0/count'.
1. 点击**TEST API**
1. 在红外传感器的前面挥挥手
1. 再次点击**TEST API**
1. 这时屏幕上的数字应该变了。

我们也可以把数据写到板子上。

1. 选择’PUT'选项卡
1. 在’resource-path’部分，选择‘/led/0/permanent_status'。
1. 在*PUT Data tab*中，键入’1'。
1. 点击 **TEST API**
1. 确认LED是亮的。
1. 回到'Parameters'
1. 在 ‘resource-path'选项下，选择‘/led/0/color'.
1. 在'PUT Data’里，键入’16711935’。
1. 按**TEST API**
1. 这时LED灯就会变成粉红色。
1. 把'permanent_status'变回 '0'

## 4. 通过代码与设备互动

我们同时也可以通过代码与设备互动。

1. 安装一个较新版本的 [node.js](https://nodejs.org) 4.0以上。
1. [把本例的源代码下载下来，并解压。](https://github.com/ARMmbed/workshop-mbed-connect-asia-lights/archive/master.zip)
1. 打开一个终端或者命令行。
1. 进入你下载解压的那个文件夹`workshop-mbed-connect-asia-lights`。
1. 在命令行执行`cd 4_from_code`
1. 执行`npm install`
1. 在编辑器里面打开`main.js`。

接下来我们需要一个接入端口的密钥 (API key)。

1. 进入[Access Keys](https://connector.mbed.com/#accesskeys)页面。
1. 点击**Generate new key**以生成新的密钥。
1. 复制密钥，粘贴到`main.js`的第一行中去。
1. 回到命令行。
1. 执行`node main.js`

你会看到以下的输出：

```
Found 1 lights [ { name: '2bffcc03-05a6-4921-b345-ebddb52f6f71',
    type: 'light-system',
    status: 'ACTIVE' } ]
```

现在我们可以订阅运动这个变量，每次变量改变我们都能得到消息。。。在'YOUR CODE HERE’这里，把下面的代码复制进去。

```js
    api.putResourceSubscription(endpoint, '/pir/0/count', function(err) {
      console.log('Set subscription for pir count', err);
    });
```

1. 重启程序：从命令行，按CTRL+C退出，然后重新运行之前的命令。
1. 把手在传感器前面挥一挥
1. 你将会在命令行里看到相应的输出。

我们现在就可以控制我们的板子了，在'YOUR CODE HERE’的地方，把下面的代码复制进去：

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

并再次运行

## 5. 一个应用

我们现在将做一个简单的网页应用来控制我们的LED灯。

1.  在你的终端命令行里，进入`5_an_app`文件夹。
1. 运行 `npm install`
1. 打开 `main.js`，并把你刚才的接入端口的密钥 (API key)再次粘贴进去。
1. 运行`node main.js`
1. 输出应该如下：


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

1. 打开浏览器，前往http://localhost:5265。现在你就可以和自己的板子互动了。
    * 取色器在SAFARI里不能使用，请用火狐浏览器，或者Chrome浏览器。
1. 我们也能让connector每次传感器被触发的时候，给你发一条消息。打开`5_an_app/views/index.html`然后在 'YOUR CODE HERE’ 部分，把下面的的代码粘贴进去。

```js
    // Movement detected! This is an event sent by the server...
    socket.on('change-count', function(endpoint, count) {
      // At the bottom of the page show a notification :-)
      showNotification('Movement detected at ' + endpoint);
    });
```

*(刷新网页)*

### 在手机上

你也可以在手机上看到你刚做的网页：

1. 找到类似于`en0 192.168.1.11`那一行，那就是你的IP地址。
1. 把你的手机连接到和电脑同一个无线网络。
1. 在你的手机上，前往http://你的IP:5265 (例如 http://192.168.1.11:5625).
1. 你将会看到和电脑上同样的一个控制灯的应用。
1. 如果你在手机应用上改变设置，电脑应用上也会实时更新。
1. 如果你在手机上用的是Chrome，点击“添加到桌面”就可以把这个网页应用和本地应用一样运行。
