// Emulated Arduino hardware for the BoopQuest sketch: clock, buttons, buzzer, backlight.

export const HIGH = 1;
export const LOW = 0;
export const INPUT = 0;
export const OUTPUT = 1;

const BUZZER_PIN = 10;
const DEBOUNCER_PIN = 13;
const BACKLIGHT_PIN = 6;
const FIRST_BUTTON_PIN = 7;
const MAX_LAG_MS = 100;

// Virtual millisecond clock. The runner calls loop() once per elapsed real millisecond, so the
// sketch observes every value of millis(), like it did on a fast-looping microcontroller.
let vms = 0;
let epoch = 0;
const realMs = () => performance.now() - epoch;
const sleep = (ms) => new Promise((resolve) => setTimeout(resolve, ms));

export function millis() {
  return vms;
}

export async function delay(ms) {
  vms += ms;
  while (realMs() < vms) {
    await sleep(vms - realMs());
  }
}

export async function run(setup, loop) {
  epoch = performance.now();
  vms = 0;
  await setup();
  for (;;) {
    const now = Math.floor(realMs());
    if (now - vms > MAX_LAG_MS) {
      vms = now - MAX_LAG_MS; // tab was backgrounded or stalled; don't replay the gap
    }
    while (vms < now) {
      await loop();
      vms++;
    }
    await sleep(1);
  }
}

// Buttons. The device powers its buttons through the debouncer pin and the sketch drops that pin
// for 300ms after every change, so a held button reads as one press followed by auto-repeat.
const LATCH_MS = 100;
export const buttons = [false, false, false];
// A press stays readable until the sketch has seen it, or briefly after release. Browsers can hold
// back timers for the whole of a short tap, so otherwise the loop might never see the button down.
const latchUntil = [0, 0, 0];
let debouncer = LOW;

export function setButton(i, held) {
  if (held && !buttons[i]) {
    latchUntil[i] = Infinity;
  } else if (!held && latchUntil[i] === Infinity) {
    latchUntil[i] = performance.now() + LATCH_MS;
  }
  buttons[i] = held;
}

export function pinMode() {}

export function digitalWrite(pin, value) {
  if (pin === DEBOUNCER_PIN) {
    debouncer = value;
  }
}

export function digitalRead(pin) {
  const i = pin - FIRST_BUTTON_PIN;
  if (debouncer === HIGH && (buttons[i] || performance.now() < latchUntil[i])) {
    latchUntil[i] = 0;
    return HIGH;
  }
  return LOW;
}

// PWM backlight, 0-255.
export let backlight = 0;

export function analogWrite(pin, value) {
  if (pin === BACKLIGHT_PIN) {
    backlight = value;
  }
}

export function analogRead() {
  return Math.floor(Math.random() * 1024);
}

export function random(max) {
  return Math.floor(Math.random() * max);
}

export function randomSeed() {}

// Piezo buzzer: one square-wave oscillator that tone()/noTone() gate on and off.
const VOLUME = 0.05;
let audio = null;
let osc = null;
let gain = null;
let toneFreq = 0;

export function initAudio() {
  if (audio) {
    audio.resume();
    return;
  }
  audio = new AudioContext();
  osc = audio.createOscillator();
  osc.type = 'square';
  gain = audio.createGain();
  gain.gain.value = 0;
  osc.connect(gain).connect(audio.destination);
  osc.start();
}

export function tone(pin, frequency, duration) {
  if (pin !== BUZZER_PIN || !audio) {
    return; // sound OFF in the menu moves the buzzer to a pin with nothing on it
  }
  const t = audio.currentTime;
  gain.gain.cancelScheduledValues(t);
  if (frequency !== toneFreq) {
    osc.frequency.setValueAtTime(frequency, t);
    toneFreq = frequency;
  }
  gain.gain.setValueAtTime(VOLUME, t);
  if (duration !== undefined) {
    gain.gain.setValueAtTime(0, t + duration / 1000);
  }
}

export function noTone(pin) {
  if (pin !== BUZZER_PIN || !audio) {
    return;
  }
  const t = audio.currentTime;
  gain.gain.cancelScheduledValues(t);
  gain.gain.setValueAtTime(0, t);
}
