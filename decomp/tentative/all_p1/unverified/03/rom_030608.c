/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x030608.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

short candidate_30608(void)

{
  short sVar1;
  undefined2 uVar2;
  int extraout_A0;
  int unaff_A6;

  *(undefined1 *)(unaff_A6 + 0x60) = 0;
  *(undefined2 *)(unaff_A6 + 0x7e) = 0xf;
  if (*(short *)(unaff_A6 + 0x82) != 0) {
    candidate_40bc();
    *(undefined2 *)(extraout_A0 + 0x80) = *(undefined2 *)(unaff_A6 + 0x80);
    *(undefined2 *)(extraout_A0 + 0x82) = *(undefined2 *)(unaff_A6 + 0x82);
    sVar1 = *(short *)(unaff_A6 + 0x82);
    if (*(short *)(unaff_A6 + 0x84) < sVar1) {
      *(undefined2 *)(unaff_A6 + 0x7e) = 0xf;
    }
    else {
      uVar2 = 0x3c;
      if (*(short *)(unaff_A6 + 0x84) == 2) {
        if (sVar1 != 2) {
          uVar2 = 0x3d;
        }
      }
      else if ((*(byte *)(unaff_A6 + 0x8f) & 1) != 0) {
        uVar2 = 0x3d;
      }
      *(undefined2 *)(unaff_A6 + 0x7e) = uVar2;
      candidate_9f88();
      if (*(short *)(unaff_A6 + 0x7e) == 0x3c) {
        candidate_9ea8();
        sVar1 = candidate_4256();
      }
      else {
        candidate_9ea8();
        sVar1 = candidate_4256();
      }
    }
    return sVar1;
  }
  return *(short *)(unaff_A6 + 0x82);
}
