/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06d3e8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6d3e8(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;
  undefined1 uVar3;

  candidate_4256();
  *(undefined2 *)(unaff_A6 + 0x1c) = 0x1e;
  *unaff_A6 = 0x6d404;
  sVar1 = *(short *)(unaff_A6 + 0x1c);
  uVar3 = sVar1 == 0;
  sVar1 = sVar1 + -1;
  *(short *)(unaff_A6 + 0x1c) = sVar1;
  if (sVar1 < 0) {
    candidate_4256();
    *unaff_A6 = 0x6d442;
    uVar2 = 0;
    uVar3 = 0;
    candidate_1436a();
    candidate_427a();
    if (!(bool)uVar2) {
      *unaff_A6 = candidate_6d46c;
      uVar3 = 0;
    }
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
      *unaff_A6 = candidate_6d4ca;
      uVar3 = 0;
    }
  }
  else {
    candidate_1436a();
    candidate_427a();
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
      *unaff_A6 = candidate_6d4ca;
      uVar3 = 0;
    }
  }
  candidate_4c24();
  if ((bool)uVar3) {
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      uVar3 = 0;
    }
    else {
      *unaff_A6 = candidate_6d57a;
      uVar3 = 0;
    }
  }
  candidate_1945c();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_6d7c2;
  }
  return;
}
