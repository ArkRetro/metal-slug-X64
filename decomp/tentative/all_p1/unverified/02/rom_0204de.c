/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0204de.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_204de(void)

{
  undefined4 uVar1;
  ushort uVar2;
  ushort uVar3;
  int unaff_A6;
  bool bVar4;

  if ((*(byte *)(unaff_A6 + 0x8c) & 4) != 0) {
    if (*(short *)(unaff_A6 + 0x88) == 0) {
      if ((*(byte *)(unaff_A6 + 0x8c) & 1) == 0) {
        uVar3 = 0x80;
      }
      else {
        uVar3 = 0x40;
      }
      do {
        uVar1 = *(undefined4 *)(unaff_A6 + 0x40);
        uVar2 = *(ushort *)(unaff_A6 + 0x40);
        bVar4 = uVar2 < uVar3;
        *(ushort *)(unaff_A6 + 0x40) = uVar2 - uVar3;
        candidate_3c5cc();
        if (bVar4) {
          *(undefined2 *)(unaff_A6 + 0x88) = 0x14;
        }
        *(undefined4 *)(unaff_A6 + 0x40) = uVar1;
      } while ((*(short *)(unaff_A6 + 0x88) == 0) && (uVar3 = uVar3 - 0x10, -1 < (short)uVar3));
    }
    else {
      *(short *)(unaff_A6 + 0x88) = *(short *)(unaff_A6 + 0x88) + -1;
    }
  }
  return;
}
