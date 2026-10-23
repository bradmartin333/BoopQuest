// The sprite character that stands above the LCD and reacts to what happens in the game.

const FRAME_SIZE = 64;
// Rows of the 64px frame that hold the art: the tallest pose starts near row 10 and feet end at 48.
const ART_TOP = 10;
const FEET = 48;
const TICKS_PER_FRAME = 6;
const WALK_SPEED = 2;

const SHEETS = {
  climb: 10, damage: 2, death: 8, fall: 2, idle: 8, jump: 2, land: 2, pickup_ground: 8,
  pickup_wall: 10, punch: 6, run: 8, talk: 7, use: 13, wait: 6, walk: 8,
};

// Animation sequences played once for each game event before returning to idle.
const REACTIONS = {
  good: ['jump', 'land'],
  bad: ['damage', 'damage'],
  boop: ['pickup_ground'],
  location: ['use'],
  spooky: ['talk'],
  funky: ['run', 'run'],
  beat: ['punch', 'punch', 'jump', 'land'],
};

const images = {};

export function loadCharacter() {
  return Promise.all(Object.entries(SHEETS).map(([name, frames]) => new Promise((resolve, reject) => {
    const img = new Image();
    img.onload = () => resolve();
    img.onerror = reject;
    img.src = `res/character/${name}_${frames}.png`;
    images[name] = img;
  })));
}

let queue = [];
let anim = 'idle';
let frame = 0;
let ticks = 0;
let facing = 1;
let x = null;
let walking = 0;

function play(name) {
  anim = name;
  frame = 0;
  ticks = 0;
}

export function react(kind) {
  queue = REACTIONS[kind].slice(1);
  walking = 0;
  play(REACTIONS[kind][0]);
}

export function walk(direction) {
  queue = [];
  facing = direction;
  walking = direction;
  play('walk');
}

export function drawCharacter(ctx) {
  const { width, height } = ctx.canvas;
  const pixel = height / (FEET - ART_TOP);
  const size = FRAME_SIZE * pixel;
  const minX = size * 0.3;
  const maxX = width - size * 0.3;
  if (x === null) {
    x = width / 2;
  }

  if (walking) {
    x = Math.min(maxX, Math.max(minX, x + walking * WALK_SPEED * pixel / 3));
  }

  if (++ticks >= TICKS_PER_FRAME) {
    ticks = 0;
    if (++frame >= SHEETS[anim]) {
      frame = 0;
      walking = 0;
      if (queue.length) {
        play(queue.shift());
      } else if (anim !== 'idle') {
        play('idle');
      }
    }
  }

  ctx.clearRect(0, 0, width, height);
  ctx.imageSmoothingEnabled = false;
  ctx.save();
  ctx.translate(x, height - FEET * pixel);
  ctx.scale(facing, 1);
  ctx.drawImage(images[anim], frame * FRAME_SIZE, 0, FRAME_SIZE, FRAME_SIZE, -size / 2, 0, size, size);
  ctx.restore();
}
