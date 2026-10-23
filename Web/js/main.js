// Wires the page to the emulated hardware: input, rendering, and starting the sketch.
import { buttons, setButton, backlight, initAudio, run } from './arduino.js';
import { drawLcd } from './lcd.js';
import { loadCharacter, drawCharacter, walk } from './character.js';
import { lcd, setup, loop, collectAllBoops } from './boopquest.js';

const KEYS = {
  KeyA: 0, ArrowLeft: 0,
  KeyS: 1, ArrowDown: 1, Space: 1, Enter: 1,
  KeyD: 2, ArrowRight: 2,
};

const buttonEls = [...document.querySelectorAll('.buttons button')];
const pointers = buttonEls.map(() => new Set());
const keys = buttonEls.map(() => new Set());

function refresh(i) {
  const held = pointers[i].size > 0 || keys[i].size > 0;
  if (held && !buttons[i] && i !== 1) {
    walk(i === 0 ? -1 : 1);
  }
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
  const i = KEYS[e.code];
  if (i === undefined || e.metaKey || e.ctrlKey || e.altKey) {
    return;
  }
  e.preventDefault();
  keys[i].add(e.code);
  refresh(i);
});

window.addEventListener('keyup', (e) => {
  const i = KEYS[e.code];
  if (i !== undefined) {
    keys[i].delete(e.code);
    refresh(i);
  }
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
const sceneCanvas = document.getElementById('scene');
const lcdCtx = lcdCanvas.getContext('2d');
const sceneCtx = sceneCanvas.getContext('2d');

function fitCanvas(canvas) {
  const dpr = window.devicePixelRatio || 1;
  canvas.width = Math.round(canvas.clientWidth * dpr);
  canvas.height = Math.round(canvas.clientHeight * dpr);
}

const resize = new ResizeObserver(() => {
  fitCanvas(lcdCanvas);
  fitCanvas(sceneCanvas);
});
resize.observe(lcdCanvas);
resize.observe(sceneCanvas);

function frame() {
  drawLcd(lcdCtx, lcd, backlight);
  drawCharacter(sceneCtx);
  requestAnimationFrame(frame);
}

const startEl = document.getElementById('start');
const ready = loadCharacter().then(() => requestAnimationFrame(frame));

startEl.addEventListener('click', async () => {
  initAudio();
  startEl.hidden = true;
  await ready;
  if (new URLSearchParams(location.search).has('boops')) {
    collectAllBoops();
  }
  run(setup, loop);
}, { once: true });
