extern crate sdl2;

use sdl2::event::Event;
use sdl2::keyboard::Keycode;
use sdl2::pixels::Color;
use std::time::Duration;

struct Vec2 {
    x: f64,
    y: f64,
}

struct Hit {
    color: i32,
    side: i32,
    dist: f64,
}
struct Person {
    loc: Vec2,
    dir: Vec2,
    cam: Vec2,
}

const VERT: u32 = 600;
const HORIZ: u32 = 800;
const MAPX: usize = 8;
const MAPY: usize = 8;
const SCREEN_OFFSETX: i32 = 150;
const RECTSIZE: usize = 16;
const TSPEED: f64 = 0.10;
const W: u32 = 800;
const H: u32 = 480;

fn draw_map(
    canvas: &mut sdl2::render::Canvas<sdl2::video::Window>,
    map: [[i32; MAPX]; MAPY],
    player: &Person,
) {
    for x in 0..MAPY {
        for y in 0..MAPX {
            let color = match map[x as usize][y as usize] {
                1 => Color::RGB(255, 0, 0),
                2 => Color::RGB(0, 255, 0),
                3 => Color::RGB(0, 0, 255),
                _ => Color::RGB(0, 0, 0),
            };
            canvas.set_draw_color(color);
            canvas
                .fill_rect(sdl2::rect::Rect::new(
                    (x * RECTSIZE) as i32,
                    (y * RECTSIZE) as i32,
                    RECTSIZE as u32,
                    RECTSIZE as u32,
                ))
                .unwrap();
        }
    }

    canvas.set_draw_color(Color::RGB(255, 255, 255));
    canvas
        .fill_rect(sdl2::rect::Rect::new(
            (player.loc.x * (RECTSIZE as f64)) as i32,
            (player.loc.y * (RECTSIZE as f64)) as i32,
            4,
            4,
        ))
        .unwrap();

    canvas.set_draw_color(Color::RGB(255, 255, 255));
    canvas
        .draw_line(
            sdl2::rect::Point::new(
                (player.loc.x * (RECTSIZE as f64)) as i32,
                (player.loc.y * (RECTSIZE as f64)) as i32,
            ),
            sdl2::rect::Point::new(
                ((player.loc.x + player.dir.x) * (RECTSIZE as f64)) as i32,
                ((player.loc.y + player.dir.y) * (RECTSIZE as f64)) as i32,
            ),
        )
        .unwrap();
}

fn draw_vert_line(
    canvas: &mut sdl2::render::Canvas<sdl2::video::Window>,
    x: i32,
    start: i32,
    end: i32,
    c: i32,
    s: i32,
) {
    let side = match s {
        0 => 1,
        _ => 2,
    };

    let color = match c {
        1 => Color::RGB(255 / side, 0, 0),
        2 => Color::RGB(0, 255 / side, 0),
        3 => Color::RGB(0, 0, 255 / side),
        _ => Color::RGB(0, 0, 0),
    };
    canvas.set_draw_color(color);

    canvas
        .draw_line(
            sdl2::rect::Point::new(x, start + SCREEN_OFFSETX),
            sdl2::rect::Point::new(x, end + SCREEN_OFFSETX),
        )
        .unwrap();
}

fn walk_squares_to_find_hit(
    player: &Person,
    map: [[i32; MAPX]; MAPY],
    raydir_x: f64,
    raydir_y: f64,
) -> Hit {
    let mut mappos_x = player.loc.x as i32;
    let mut mappos_y = player.loc.y as i32;
    let mut hit = false;
    let mut side: i32 = 0;
    let mut dist_x: f64;
    let mut dist_y: f64;
    let wall_dist: f64;
    let step_x: i32;
    let step_y: i32;

    let delta_dist_x = if raydir_x == 0.0 {
        1e30
    } else {
        f64::abs(1.0 / raydir_x)
    };
    let delta_dist_y = if raydir_y == 0.0 {
        1e30
    } else {
        f64::abs(1.0 / raydir_y)
    };

    if raydir_x < 0.0 {
        step_x = -1;
        dist_x = (player.loc.x as i32 - mappos_x) as f64 * delta_dist_x;
    } else {
        step_x = 1;
        dist_x = (mappos_x as f64 + 1.0 - player.loc.x) * delta_dist_x;
    }

    if raydir_y < 0.0 {
        step_y = -1;
        dist_y = (player.loc.y as i32 - mappos_y) as f64 * delta_dist_y;
    } else {
        step_y = 1;
        dist_y = (mappos_y as f64 + 1.0 - player.loc.y) * delta_dist_y;
    }

    while !hit {
        if dist_x < dist_y {
            dist_x += delta_dist_x;
            mappos_x += step_x;
            hit = map[mappos_x as usize][mappos_y as usize] > 0;
            side = 0;
        } else {
            dist_y += delta_dist_y;
            mappos_y += step_y;
            hit = map[mappos_x as usize][mappos_y as usize] > 0;
            side = 1;
        }
    }

    if side == 0 {
        wall_dist = dist_x - delta_dist_x;
    } else {
        wall_dist = dist_y - delta_dist_y;
    }

    return Hit {
        color: map[mappos_x as usize][mappos_y as usize],
        side: side,
        dist: wall_dist,
    };
}

pub fn main() {
    let sdl_context = sdl2::init().unwrap();
    let video_subsystem = sdl_context.video().unwrap();
    let window = video_subsystem
        .window("rust-sdl2 demo", HORIZ, VERT)
        .position_centered()
        .build()
        .unwrap();
    let mut canvas = window.into_canvas().build().unwrap();
    canvas.set_draw_color(Color::RGB(0, 255, 255));
    canvas.clear();
    canvas.present();

    let mut player = Person {
        loc: Vec2 { x: 1.0, y: 2.0 },
        dir: Vec2 { x: 1.0, y: 0.0 },
        cam: Vec2 { x: 0.0, y: 0.66 },
    };

    let map = [
        [1, 1, 1, 1, 1, 1, 1, 1],
        [1, 0, 0, 0, 0, 0, 0, 1],
        [1, 3, 3, 0, 0, 0, 0, 1],
        [1, 3, 3, 0, 0, 0, 0, 1],
        [1, 0, 0, 0, 2, 2, 0, 1],
        [1, 0, 0, 0, 2, 2, 0, 1],
        [1, 0, 0, 0, 0, 0, 0, 1],
        [1, 1, 1, 1, 1, 1, 1, 1],
    ];

    let mut event_pump = sdl_context.event_pump().unwrap();
    'running: loop {
        for event in event_pump.poll_iter() {
            match event {
                Event::Quit { .. }
                | Event::KeyDown {
                    keycode: Some(Keycode::Escape),
                    ..
                } => break 'running,
                Event::KeyDown {
                    keycode: Some(Keycode::W),
                    ..
                } => {
                    player.loc.x = player.loc.x + player.dir.x * 0.1;
                    player.loc.y = player.loc.y + player.dir.y * 0.1;
                }
                Event::KeyDown {
                    keycode: Some(Keycode::S),
                    ..
                } => {
                    player.loc.x = player.loc.x + player.dir.x * -0.1;
                    player.loc.y = player.loc.y + player.dir.y * -0.1;
                } // Move backward
                Event::KeyDown {
                    keycode: Some(Keycode::A),
                    ..
                } => {
                    let old_x = player.dir.x;
                    player.dir.x = old_x * f64::cos(TSPEED) + player.dir.y * f64::sin(TSPEED);
                    player.dir.y = -old_x * f64::sin(TSPEED) + player.dir.y * f64::cos(TSPEED);
                    let old_x = player.cam.x;
                    player.cam.x = old_x * f64::cos(TSPEED) + player.cam.y * f64::sin(TSPEED);
                    player.cam.y = -old_x * f64::sin(TSPEED) + player.cam.y * f64::cos(TSPEED);
                }
                Event::KeyDown {
                    keycode: Some(Keycode::D),
                    ..
                } => {
                    let old_x = player.dir.x;
                    player.dir.x = old_x * f64::cos(-TSPEED) + player.dir.y * f64::sin(-TSPEED);
                    player.dir.y = -old_x * f64::sin(-TSPEED) + player.dir.y * f64::cos(-TSPEED);
                    let old_x = player.cam.x;
                    player.cam.x = old_x * f64::cos(-TSPEED) + player.cam.y * f64::sin(-TSPEED);
                    player.cam.y = -old_x * f64::sin(-TSPEED) + player.cam.y * f64::cos(-TSPEED);
                }
                _ => {}
            }
        }

        canvas.set_draw_color(Color::RGB(0, 0, 0));
        canvas.clear();
        draw_map(&mut canvas, map, &player);

        for x in 0..W {
            let camera_x = 2.0 * x as f64 / W as f64 - 1.0;
            let raydir_x = player.dir.x + player.cam.x * camera_x;
            let raydir_y = player.dir.y + player.cam.y * camera_x;

            let hit = walk_squares_to_find_hit(&player, map, raydir_x, raydir_y);

            let line_height = (H as f64 / hit.dist) as i32;
            let mut draw_start = -line_height / 2 + H as i32 / 2;
            if draw_start < 0 {
                draw_start = 0;
            }
            let mut draw_end = line_height / 2 + H as i32 / 2;
            if draw_end >= H as i32 {
                draw_end = H as i32 - 1;
            }

            draw_vert_line(
                &mut canvas,
                x as i32,
                draw_start,
                draw_end,
                hit.color,
                hit.side,
            )
        }

        canvas.present();
        ::std::thread::sleep(Duration::new(0, 1_000_000_000u32 / 60));
    }
}
