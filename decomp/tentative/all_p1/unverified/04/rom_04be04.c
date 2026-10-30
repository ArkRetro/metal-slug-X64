/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04be04.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4be04(void)

{
  ushort uVar1;
  short sVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_4256();
  *(undefined2 *)((int)unaff_A6 + 0x26) = 0x100;
  *(undefined2 *)(unaff_A6 + 0x22) = 0;
  *unaff_A6 = 0x4be26;
  candidate_1abc8();
  if (*(short *)(unaff_A6 + 0x22) == 0) {
    if (*(short *)((int)unaff_A6 + 0x26) == 1) {
      *(undefined1 *)((int)unaff_A6 + 0x9e) = 0xff;
      *unaff_A6 = candidate_4ba94;
      uVar3 = false;
    }
    else {
      *(undefined2 *)(unaff_A6 + 0x22) = 0;
      uVar1 = *(ushort *)((int)unaff_A6 + 0x26);
      uVar3 = (uVar1 & 1) != 0;
      *(ushort *)((int)unaff_A6 + 0x26) = uVar1 >> 1;
    }
  }
  else {
    sVar2 = *(short *)(unaff_A6 + 0x22);
    uVar3 = sVar2 == 0;
    *(short *)(unaff_A6 + 0x22) = sVar2 + -1;
  }
  candidate_4d9de();
  candidate_4d74a();
  if ((bool)uVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
  }
  return;
}
