#pragma once

#include "types.h"
#include "texture.h"


namespace Machine
{

using MemoryAddress = u16;


template <u32 screen_buffer_width, u32 screen_buffer_height>
struct Memory
{
  static constexpr r32 pixel_size = 0.5;

  static const u32 screen_buffer_size = (screen_buffer_width * screen_buffer_height) * pixel_size;
  static const u32 general_buffer_size = 0x2000;
  static const u32 size = (screen_buffer_size + general_buffer_size);

  u8 bytes[size];

  //
  // Screen buffer:
  // 128*128 pixels
  //  * 4 bit colours
  //  = 0x2000 bytes
  //
  // 1 row = 128 pixels = 0x40 bytes
  //
  // r0   = 0x0000
  // r8   = 0x0200
  // r16  = 0x0400
  // r32  = 0x0800
  // r64  = 0x1000
  // r127 = 0x2000-0x40 = 0x1fC0
  //
  // 0x2000 = 8 KiB
  //
  // User memory   = [0x0000, 0x2000)  8 KiB
  // Screen buffer = [0x2000, 0x4000)  8 KiB
  // Total = 16 KiB
  //
  // 16KiB of address space requires 2 bytes to index. (u16)
  //
  // Instructions have a set specified length of their inputs, which is the gap until the next instruction.
  // All instructions are byte aligned.
  //
  // 0x102A+2 = shorthand for the bytes [0x102A, 0x102B, 0x102C] inclusive.
  //
  // 0x0000+1 is the next instruction register.
  //
};


enum class InstructionCodes : u8
{
  NOP     = 0x0,
  ADD     = 0x1,
  SUB     = 0x2,
  MUL     = 0x3,
  DIV     = 0x4,
  IF_ZERO = 0x5,
  LOAD    = 0x6,
  STORE   = 0x7
};


enum class InstructionWidths
{
  // Size in bytes of instruction + arguments
  NOP     = sizeof(MemoryAddress)*(1 + 0),
  ADD     = sizeof(MemoryAddress)*(1 + 3),
  SUB     = sizeof(MemoryAddress)*(1 + 3),
  MUL     = sizeof(MemoryAddress)*(1 + 3),
  DIV     = sizeof(MemoryAddress)*(1 + 3),
  IF_ZERO = sizeof(MemoryAddress)*(1 + 2),
  LOAD    = sizeof(MemoryAddress)*(1 + 1),
  STORE   = sizeof(MemoryAddress)*(1 + 1)
};


namespace Reserved
{
enum : MemoryAddress
{
  NI = 0x0000,
  ScreenBuffer = 0x2000
};
} // namespace Reserved


struct Machine
{
  Memory<128, 128> memory;
};


template <typename size, u32 w, u32 h>
size *
get_ptr(Memory<w,h>& memory, MemoryAddress addr)
{
  return (size *)(memory.bytes + addr);
}


template <typename size, u32 w, u32 h>
size
get(Memory<w,h>& memory, MemoryAddress addr)
{
  return *get_ptr<size>(memory, addr);
}


template <typename size, u32 w, u32 h>
inline void
set(Memory<w,h>& memory, MemoryAddress addr, size byte_value)
{
  *get_ptr<size>(memory, addr) = byte_value;
}


template <u32 w, u32 h>
inline bool
allocate_screen_buffer_texture(Memory<w,h>& memory, Texture::Texture& texture)
{
  return Texture::allocate(texture, w, h);
}


void
advance(Machine &machine);

} // namespace Machine