// HD44780 16x2 character LCD: the subset of the Arduino LiquidCrystal API the sketch uses, plus a
// canvas renderer that draws it as a blue backlit dot-matrix panel.
import { FONT_A00 } from './font_a00.js';

const COLS = 16;
const ROWS = 2;
const ROW_OFFSETS = [0x00, 0x40];
const LINE_LENGTH = 0x28; // each line holds 40 characters of DDRAM; only the first 16 are visible

export class LiquidCrystal {
  constructor() {
    this.ddram = new Uint8Array(0x80).fill(32);
    this.cgram = Array.from({ length: 8 }, () => new Uint8Array(8));
    this.address = 0;
    this.visible = true;
  }

  begin() {
    this.clear();
  }

  clear() {
    this.ddram.fill(32);
    this.address = 0;
  }

  setCursor(col, row) {
    if (row >= ROWS) {
      row = ROWS - 1;
    }
    this.address = (col + ROW_OFFSETS[row]) & 0x7f;
  }

  write(code) {
    this.ddram[this.address] = code & 0xff;
    // Writing past the end of one line continues on the other, as the controller does.
    if (this.address === LINE_LENGTH - 1) {
      this.address = 0x40;
    } else if (this.address === 0x40 + LINE_LENGTH - 1) {
      this.address = 0x00;
    } else {
      this.address = (this.address + 1) & 0x7f;
    }
  }

  print(text) {
    for (const ch of text) {
      const code = ch.charCodeAt(0);
      if (code === 0) {
        break;
      }
      this.write(code);
    }
  }

  createChar(location, rows) {
    this.cgram[location & 7] = Uint8Array.from(rows);
  }

  display() {
    this.visible = true;
  }

  noDisplay() {
    this.visible = false;
  }

  glyphRow(code, row) {
    return code < 16 ? this.cgram[code & 7][row] : FONT_A00[code * 8 + row];
  }
}

// Panel geometry in dot units: 5x8 dots per character, one dot of space between characters.
const CHAR_W = 5;
const CHAR_H = 8;
const GAP = 1;
const MARGIN = 3;
export const PANEL_DOTS_W = MARGIN * 2 + COLS * CHAR_W + (COLS - 1) * GAP;
export const PANEL_DOTS_H = MARGIN * 2 + ROWS * CHAR_H + (ROWS - 1) * GAP;

function mix(a, b, t) {
  return a.map((v, i) => Math.round(v + (b[i] - v) * t));
}

const UNLIT_BG = [8, 14, 40];
const LIT_BG = [36, 84, 235];
const UNLIT_INK = [40, 52, 88];
const LIT_INK = [232, 240, 255];

export function drawLcd(ctx, lcd, backlight) {
  const { width, height } = ctx.canvas;
  const dot = Math.min(width / PANEL_DOTS_W, height / PANEL_DOTS_H);
  const ox = (width - dot * PANEL_DOTS_W) / 2;
  const oy = (height - dot * PANEL_DOTS_H) / 2;
  const level = backlight <= 0 ? 0 : 0.45 + 0.55 * Math.sqrt(backlight / 255);
  const bg = mix(UNLIT_BG, LIT_BG, level);
  const ink = mix(UNLIT_INK, LIT_INK, level);

  ctx.fillStyle = `rgb(${bg})`;
  ctx.fillRect(0, 0, width, height);

  const size = dot * 0.86;
  for (let row = 0; row < ROWS; row++) {
    for (let col = 0; col < COLS; col++) {
      const code = lcd.ddram[ROW_OFFSETS[row] + col];
      const cx = ox + (MARGIN + col * (CHAR_W + GAP)) * dot;
      const cy = oy + (MARGIN + row * (CHAR_H + GAP)) * dot;
      for (let py = 0; py < CHAR_H; py++) {
        const bits = lcd.visible ? lcd.glyphRow(code, py) : 0;
        for (let px = 0; px < CHAR_W; px++) {
          const on = bits & (0x10 >> px);
          ctx.fillStyle = on ? `rgb(${ink})` : 'rgba(255, 255, 255, 0.06)';
          ctx.fillRect(cx + px * dot, cy + py * dot, size, size);
        }
      }
    }
  }
}
