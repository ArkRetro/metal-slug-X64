/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03248a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3248a(void)

{
  undefined4 *unaff_A6;

  *(undefined2 *)((int)unaff_A6 + 0x6e) = 8;
  *(undefined1 *)((int)unaff_A6 + 0x7d) = 0;
  unaff_A6[0x14] = 0x32194;
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8800;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8800;
  candidate_9ea8();
  if (*(short *)(unaff_A6 + 0xc) < 0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  candidate_4256();
  *unaff_A6 = candidate_32504;
  return;
}
