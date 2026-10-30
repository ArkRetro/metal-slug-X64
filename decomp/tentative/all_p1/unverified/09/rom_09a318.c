/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09a318.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9a318(void)

{
  undefined4 extraout_A0;
  int in_A0;
  undefined4 *unaff_A6;
  undefined1 uVar1;

  *(undefined2 *)(unaff_A6 + 0x1d) = 0xffff;
  if (*(short *)(in_A0 + 0x3c) < *(short *)(unaff_A6 + 0xf)) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  }
  else {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  candidate_4256();
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  unaff_A6[0xc] = 0;
  *unaff_A6 = 0x9a2d8;
  uVar1 = 0;
  candidate_9ac86();
  candidate_427a();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_9a322;
    uVar1 = *(ushort *)(unaff_A6 + 0x1d) < 0xfffe;
    if (*(ushort *)(unaff_A6 + 0x1d) == 0xfffe) {
      uVar1 = false;
      candidate_1aeb0();
      if ((bool)uVar1) {
        *(undefined2 *)(unaff_A6 + 0x1d) = 0xffff;
        *(undefined4 *)((int)unaff_A6 + 0x76) = extraout_A0;
        *unaff_A6 = candidate_ddce6;
        return;
      }
    }
  }
  candidate_c8bec();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_9a5ee;
  }
  return;
}
