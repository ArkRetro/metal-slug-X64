/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01e19e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1e19e(void)

{
  short in_D0w;
  undefined1 in_D1b;
  byte unaff_D2b;
  int extraout_A0;
  int extraout_A0_00;
  int iVar1;
  int iVar2;
  int unaff_A6;
  bool bVar3;

  if (in_D0w == 0) {
    iVar1 = 0x10c340;
    iVar2 = 0x10c348;
  }
  else {
    iVar2 = 0x10c340;
    iVar1 = 0x10c348;
  }
  if (*(char *)(iVar1 + 6) == '\0') {
    *(undefined1 *)(iVar1 + 7) = 1;
    if (*(char *)(iVar2 + 7) == '\0') {
      candidate_120f8();
    }
    bVar3 = false;
    candidate_40bc();
    iVar2 = extraout_A0;
    if (bVar3) {
      candidate_1b220();
      iVar2 = extraout_A0_00;
    }
    *(undefined1 *)(iVar2 + 0x5e) = *(undefined1 *)(unaff_A6 + 0x5e);
    *(short *)(iVar2 + 0x6e) = in_D0w;
    *(undefined1 *)(iVar2 + 0x70) = in_D1b;
    *(undefined1 *)(iVar1 + 6) = 1;
  }
  *(byte *)(iVar1 + 5) = unaff_D2b & 0x7f;
  *(undefined1 *)(iVar1 + 4) = in_D1b;
  return;
}
