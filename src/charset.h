// Meteo Clock - cool clock with meteostation on Arduino
// Copyright (C) 2022 WolodiaM

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
#define BLOCK 0
#define BLOCKUP 1
#define BLOCKDOWN 2
#define RIGHTUP 4
#define RIGHTDOWN 6
#define LEFTUP 5
#define LEFTDOWN 7
byte block[] = {
    0x1F,
    0x1F,
    0x1F,
    0x1F,
    0x1F,
    0x1F,
    0x1F,
    0x1F};

byte blockUp[] = {
    0x1F,
    0x1F,
    0x1F,
    0x1F,
    0x00,
    0x00,
    0x00,
    0x00};

byte blockDown[] = {
    0x00,
    0x00,
    0x00,
    0x00,
    0x1F,
    0x1F,
    0x1F,
    0x1F};

byte upRight[] = {
    0x1F,
    0x1F,
    0x0F,
    0x0F,
    0x07,
    0x07,
    0x03,
    0x03};

byte upLeft[] = {
    0x1F,
    0x1F,
    0x1E,
    0x1E,
    0x1C,
    0x1C,
    0x18,
    0x18};

byte downRight[] = {
    0x03,
    0x03,
    0x07,
    0x07,
    0x0F,
    0x0F,
    0x1F,
    0x1F};

byte downLeft[] = {
    0x18,
    0x18,
    0x1C,
    0x1C,
    0x1E,
    0x1E,
    0x1F,
    0x1F};