/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x078a58.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_78a58(void)

{
  short sVar1;
  ushort uVar2;
  undefined4 *unaff_A6;
  bool bVar3;
  undefined1 uVar4;

  if (*(char *)(unaff_A6 + 0x27) == '\0') {
    uVar2 = candidate_29c6();
    if ((uVar2 & 1) != 0) {
      candidate_1b1dc();
      *(undefined4 *)((int)unaff_A6 + 0x62) = 0xfffe0000;
      if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
        *(int *)((int)unaff_A6 + 0x62) = -*(int *)((int)unaff_A6 + 0x62);
      }
      candidate_4256();
      *unaff_A6 = 0x78af2;
      candidate_78cf4();
      candidate_427a();
      if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
        uVar2 = *(short *)(unaff_A6[3] + 0x3c) - *(short *)(unaff_A6 + 0xf);
        if (0x1f < (short)uVar2) {
          *unaff_A6 = candidate_78b70;
        }
        uVar4 = uVar2 < 0x20;
      }
      else {
        uVar2 = *(short *)(unaff_A6 + 0xf) - *(short *)(unaff_A6[3] + 0x3c);
        if (*(char *)(unaff_A6 + 0x27) == '\0') {
          uVar4 = uVar2 < 0x30;
          bVar3 = SBORROW2(uVar2,0x30);
          sVar1 = -0x30;
        }
        else {
          uVar4 = uVar2 < 0x28;
          bVar3 = SBORROW2(uVar2,0x28);
          sVar1 = -0x28;
        }
        if (bVar3 == (short)(uVar2 + sVar1) < 0) {
          *unaff_A6 = candidate_78b70;
          uVar4 = false;
        }
      }
      candidate_78fc4();
      candidate_78fa2();
      candidate_78d54();
      if ((bool)uVar4) {
        *unaff_A6 = 0x77e5c;
        uVar4 = 0;
      }
      candidate_78d7a();
      if ((bool)uVar4) {
        *unaff_A6 = 0x77e5c;
      }
      return;
    }
  }
  candidate_1b1dc();
  candidate_4256();
  *unaff_A6 = 0x78a8a;
  uVar4 = 0;
  candidate_78cf4();
  candidate_427a();
  if ((bool)uVar4) {
    *unaff_A6 = 0x78ac0;
    uVar4 = 0;
  }
  candidate_78fa2();
  candidate_78d54();
  if ((bool)uVar4) {
    *unaff_A6 = 0x77e5c;
    uVar4 = 0;
  }
  candidate_78d7a();
  if ((bool)uVar4) {
    *unaff_A6 = 0x77e5c;
  }
  return;
}
