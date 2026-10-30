/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d7962.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d7962(void)

{
  short sVar1;
  undefined4 *unaff_A6;

  sVar1 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x25) = *(undefined2 *)((short)(sVar1 * 4) + 0x2cbe84);
  sVar1 = *(short *)((short)(sVar1 * 4 + 2) + 0x2cbe84);
  *(short *)(unaff_A6 + 0x2a) = sVar1;
  unaff_A6[0xc] = (int)((uint)(ushort)-sVar1 << 0x10) >> 8;
  *(undefined2 *)((int)unaff_A6 + 0x76) = 0;
  *(byte *)((int)unaff_A6 + 0x61) = *(byte *)((int)unaff_A6 + 0x61) | 0x20;
  *unaff_A6 = 0xd79b2;
  *(short *)((int)unaff_A6 + 0x76) = *(short *)((int)unaff_A6 + 0x76) + -1;
  if ((*(short *)((int)unaff_A6 + 0x76) < 1) && ((*(byte *)((int)unaff_A6 + 0x61) & 0x20) != 0)) {
    *(undefined2 *)((int)unaff_A6 + 0x76) = *(undefined2 *)(unaff_A6 + 0x25);
    if (*(char *)(unaff_A6 + 0x20) == -1) {
      *(undefined1 *)((int)unaff_A6 + 0x89) = 0xe;
    }
    else {
      *(undefined1 *)((int)unaff_A6 + 0x89) = 0xd;
    }
  }
  candidate_1ac38();
  candidate_d90f8();
  if (*(short *)(unaff_A6 + 0xf) < -0x1d) {
    *unaff_A6 = candidate_d7a08;
  }
  candidate_4c24();
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  }
  return;
}
