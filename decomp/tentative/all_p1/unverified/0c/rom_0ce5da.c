/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ce5da.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_ce5da(void)

{
  undefined1 uVar1;
  short sVar2;
  int unaff_A6;
  undefined1 in_CF;

  candidate_4c24();
  if (((bool)in_CF) &&
     (*(undefined1 *)(unaff_A6 + 0x5a) = 0,
     *(short *)(unaff_A6 + 0x4e) <= *(short *)(unaff_A6 + 0x76))) {
    if (*(char *)(unaff_A6 + 0x73) != *(char *)(unaff_A6 + 0x81)) {
      *(char *)(unaff_A6 + 0x81) = *(char *)(unaff_A6 + 0x73);
      candidate_1da40();
    }
    uVar1 = 2;
    sVar2 = 0;
    if (*(short *)(unaff_A6 + 0x78) != *(short *)(unaff_A6 + 0x76)) {
      uVar1 = 1;
      sVar2 = *(short *)(unaff_A6 + 0x78);
    }
    *(short *)(unaff_A6 + 0x76) = sVar2;
    *(undefined1 *)(unaff_A6 + 0x73) = uVar1;
  }
  return;
}
