
(function() {
/**
 * color-picker custom element
 */

var colorPicker = window.ColorPicker = function(width, height) {
  this.attrWidth = width;
  this.attrHeight = height;

  this.el = document.createElement('div');
  this.el.style.width = width + 'px';
  this.el.style.height = height + 'px';
};

colorPicker.prototype.onMouseDown = function(e) {
    this.onMouseMove(e);
    this.onmousemove = this.onMouseMove.bind(this);
}

colorPicker.prototype.onMouseUp = function(e) {
    this.el.onmousemove = null;
}

colorPicker.prototype.onTouchStart = function(e) {
    this.onTouchMove(e);
    this.ontouchmove = this.onTouchMove.bind(this);
}

colorPicker.prototype.onTouchEnd = function(e) {
    this.el.removeEventListener('touchmove', this.onTouchMove.bind(this));
    this.ontouchmove = null;
}

colorPicker.prototype.onTouchMove = function(e) {
    var touch = e.touches[0];
    this.onColorSelect(e, {
        x: touch.clientX,
        y: touch.clientY
    });
}

colorPicker.prototype.onMouseMove = function(e) {
    e.preventDefault();
    if (this.mouseMoveIsThrottled) {
        this.mouseMoveIsThrottled = false;
        this.onColorSelect(e);
        setTimeout(function() {
            this.mouseMoveIsThrottled = true;
        }.bind(this), 100);
    }
}

colorPicker.prototype.onColorSelect = function(e, coords) {

    if (this.context) {

        coords = coords || this.relativeMouseCoordinates(e);
        var data = this.context.getImageData(coords.x, coords.y, 1, 1).data;

        this.setColor({
            r: data[0],
            g: data[1],
            b: data[2]
        });
    }
};

colorPicker.prototype.pickerDraw = function() {

    this.canvas = document.createElement('canvas');
    this.canvas.setAttribute("width", this.width);
    this.canvas.setAttribute("height", this.height);
    this.canvas.setAttribute("style", "cursor:crosshair");
    this.canvas.setAttribute("class", "simpleColorPicker");

    this.context = this.canvas.getContext('2d');

    var colorGradient = this.context.createLinearGradient(0, 0, this.width, 0);
    colorGradient.addColorStop(0, "rgb(255,0,0)");
    colorGradient.addColorStop(0.16, "rgb(255,0,255)");
    colorGradient.addColorStop(0.32, "rgb(0,0,255)");
    colorGradient.addColorStop(0.48, "rgb(0,255,255)");
    colorGradient.addColorStop(0.64, "rgb(0,255,0)");
    colorGradient.addColorStop(0.80, "rgb(255,255,0)");
    colorGradient.addColorStop(1, "rgb(255,0,0)");
    this.context.fillStyle = colorGradient;
    this.context.fillRect(0, 0, this.width, this.height);

    var bwGradient = this.context.createLinearGradient(0, 0, 0, this.height);
    bwGradient.addColorStop(0, "rgba(255,255,255,1)");
    bwGradient.addColorStop(0.5, "rgba(255,255,255,0)");
    bwGradient.addColorStop(0.5, "rgba(0,0,0,0)");
    bwGradient.addColorStop(1, "rgba(0,0,0,1)");

    this.context.fillStyle = bwGradient;
    this.context.fillRect(0, 0, this.width, this.height);

}

colorPicker.prototype.setColor = function(rgb) {

    //save calculated color
    this.color = {
        hex: this.rgbToHex(rgb),
        rgb: rgb
    };

    //update element attribute
    this.el.setAttribute('color', this.color.hex);

    //broadcast color selected event
    var event = new CustomEvent('colorselected', {
        detail: {
            rgb: this.color.rgb,
            hex: this.color.hex
        }
    });

    this.el.dispatchEvent(event);
}

/**
 * given red, green, blue values, return the equivalent hexidecimal value
 * base source: http://stackoverflow.com/a/5624139
 */
colorPicker.prototype.componentToHex = function(c) {
    var hex = c.toString(16);
    return hex.length == 1 ? "0" + hex : hex;
};

colorPicker.prototype.rgbToHex = function(color) {
    return "#" + this.componentToHex(color.r) + this.componentToHex(color.g) + this.componentToHex(color.b);
};

/**
 * given a mouse click event, return x,y coordinates relative to the clicked target
 * @returns object with x, y values
 */
colorPicker.prototype.relativeMouseCoordinates = function(e) {

    var x = 0, y = 0;

    if (this.canvas) {
        var rect = this.canvas.getBoundingClientRect();
        x = e.clientX - rect.left;
        y = e.clientY - rect.top;
    }

    return {
        x: x,
        y: y
    };
};

colorPicker.prototype.createdCallback = function(e) {

    //parse attributes
    var attrs = {
        width: this.attrWidth,
        height: this.attrHeight
    };

    //initialization
    this.canvas = null;
    this.context = null;
    this.color = null;
    this.mouseMoveIsThrottled = true;
    this.width = attrs.width || 300;
    this.height = attrs.height || 300;

    //create UI
    this.pickerDraw();
    this.el.appendChild(this.canvas);

    ///event listeners
    this.el.onmousedown = this.onMouseDown.bind(this);
    this.el.onmouseup = this.onMouseUp.bind(this);
    this.el.ontouchstart = this.onTouchStart.bind(this);
    this.el.ontouchend = this.onTouchEnd.bind(this);

};

})();