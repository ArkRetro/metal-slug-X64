/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01edf6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1edf6(void)

{
  undefined4 uVar1;
  int unaff_A6;
  bool bVar2;
  undefined1 uVar3;

  bVar2 = false;
  if ((*(char *)(unaff_A6 + 0x84) != '\0') || (candidate_1eed8(), bVar2)) {
LAB_0001ee66:
    *(undefined1 *)(unaff_A6 + 0x85) = 1;
  }
  else {
    *(undefined1 *)(unaff_A6 + 0x85) = 0;
    if ((*(byte *)(unaff_A6 + 0x15) & 2) == 0) {
      uVar1 = 0x100000;
    }
    else {
      uVar1 = 0xfff00000;
    }
    *(undefined4 *)(unaff_A6 + 0x30) = 0;
    *(undefined4 *)(unaff_A6 + 0x34) = uVar1;
    do {
      uVar3 = false;
      candidate_14906();
      if ((bool)uVar3) goto LAB_0001ee66;
      candidate_1ac38();
      candidate_427a();
      candidate_1eed8();
      if ((bool)uVar3) goto LAB_0001ee66;
    } while ((0x100 < *(short *)(unaff_A6 + 0x40)) && (*(short *)(unaff_A6 + 0x40) < 0x200));
  }
  return;
}
