(function() {
  /*global changeColor */

  var cpOverlay = document.querySelector('#color-picker-overlay');
  // this is needed by color-picker component unfortunately (does reflow)
  var vw = window.innerWidth * 0.8 | 0;

  // so this thing reads its attributes when creating, not when adding to body
  // that's a mess... dont feel like patching the library

  var cp = new ColorPicker(vw, vw);
  cp.createdCallback();

  cpOverlay.appendChild(cp.el);

  cp.el.addEventListener('colorselected', function(e) {
    if (!cpOverlay.dataset.endpoint) return console.error('No data-endpoint attribute found...');

    var rgb = e.detail.rgb;
    var v = (rgb.r << 16) + (rgb.g << 8) + rgb.b;

    changeColor(cpOverlay.dataset.endpoint, v);
  });

  cpOverlay.onclick = function(e) {
    if (e.target !== e.currentTarget) return;
    this.style.display = 'none';
  };
})();
