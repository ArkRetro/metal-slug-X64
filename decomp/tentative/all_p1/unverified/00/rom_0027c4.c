/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0027c4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_27c4(void)

{
  byte in_D0b;
  byte *in_A0;

  if (3 < in_D0b) {
    __m68k_trap(0xf);
  }
  *in_A0 = *(byte *)((short)(ushort)(byte)(in_D0b + (*in_A0 & 3) * '\x04') + 0x27b4);
  return;
}
