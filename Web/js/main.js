// Wires the page to the emulated hardware: input, rendering, and starting the sketch.
import { setButton, backlight, initAudio, run } from './arduino.js';
import { drawLcd } from './lcd.js';
import { lcd, setup, loop, collectAllBoops, isDiving } from './boopquest.js';

const KEYS = {
  KeyA: 0, ArrowLeft: 0,
  KeyS: 1, ArrowDown: 1, Space: 1, Enter: 1,
  KeyD: 2, ArrowRight: 2,
};
// During the dive the device is turned sideways: Left is at the bottom, Right at the top.
const DIVE_KEYS = { ...KEYS, ArrowDown: 0, ArrowLeft: 1, ArrowRight: 1, ArrowUp: 2 };
let diving = false;

const buttonEls = [...document.querySelectorAll('.buttons button')];
const pointers = buttonEls.map(() => new Set());
const keys = buttonEls.map(() => new Set());

function refresh(i) {
  const held = pointers[i].size > 0 || keys[i].size > 0;
  setButton(i, held);
  buttonEls[i].classList.toggle('pressed', held);
}

buttonEls.forEach((el, i) => {
  el.addEventListener('pointerdown', (e) => {
    e.preventDefault();
    el.setPointerCapture(e.pointerId);
    pointers[i].add(e.pointerId);
    refresh(i);
  });
  const release = (e) => {
    pointers[i].delete(e.pointerId);
    refresh(i);
  };
  el.addEventListener('pointerup', release);
  el.addEventListener('pointercancel', release);
  el.addEventListener('lostpointercapture', release);
});

window.addEventListener('keydown', (e) => {
  const i = (diving ? DIVE_KEYS : KEYS)[e.code];
  if (i === undefined || e.metaKey || e.ctrlKey || e.altKey) {
    return;
  }
  e.preventDefault();
  keys[i].add(e.code);
  refresh(i);
});

window.addEventListener('keyup', (e) => {
  // The layout can rotate while a key is held, so release it from whichever button it pressed.
  keys.forEach((held, i) => {
    if (held.delete(e.code)) {
      refresh(i);
    }
  });
});

window.addEventListener('blur', () => {
  buttonEls.forEach((_, i) => {
    pointers[i].clear();
    keys[i].clear();
    refresh(i);
  });
});

document.addEventListener('contextmenu', (e) => e.preventDefault());

const lcdCanvas = document.getElementById('lcd');
const lcdCtx = lcdCanvas.getContext('2d');

function fitCanvas(canvas) {
  const dpr = window.devicePixelRatio || 1;
  canvas.width = Math.round(canvas.clientWidth * dpr);
  canvas.height = Math.round(canvas.clientHeight * dpr);
}

const resize = new ResizeObserver(() => {
  fitCanvas(lcdCanvas);
});
resize.observe(lcdCanvas);

function frame() {
  if (diving !== isDiving()) {
    diving = !diving;
    document.body.classList.toggle('diving', diving);
  }
  drawLcd(lcdCtx, lcd, backlight);
  requestAnimationFrame(frame);
}

const startEl = document.getElementById('start');
requestAnimationFrame(frame);

startEl.addEventListener('click', () => {
  initAudio();
  startEl.hidden = true;
  if (new URLSearchParams(location.search).has('boops')) {
    collectAllBoops();
  }
  run(setup, loop);
}, { once: true });
