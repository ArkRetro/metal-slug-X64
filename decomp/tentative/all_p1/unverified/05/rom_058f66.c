/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x058f66.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_58f66(void)

{
  short sVar1;
  undefined2 uVar2;
  byte bVar3;
  undefined4 *unaff_A6;

  uVar2 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x86) = uVar2;
  *(undefined2 *)(unaff_A6 + 0x20) = 6;
  uVar2 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x86) = uVar2;
  candidate_59c2c();
  if ((*(byte *)((int)unaff_A6 + 0x8e) & 1) == 0) {
    candidate_40bc();
  }
  else {
    candidate_40bc();
  }
  candidate_1acf2();
  *unaff_A6 = 0x58ff2;
  candidate_59214();
  sVar1 = *(short *)((int)unaff_A6 + 0x86) + -1;
  *(short *)((int)unaff_A6 + 0x86) = sVar1;
  if (sVar1 == 0) {
    *(char *)((int)unaff_A6 + 0x8e) = *(char *)((int)unaff_A6 + 0x8e) + '\x01';
    bVar3 = candidate_260c8();
    if (*(byte *)((int)unaff_A6 + 0x8e) < bVar3) {
      *unaff_A6 = candidate_58efc;
    }
    else {
      *(undefined1 *)((int)unaff_A6 + 0x91) = 0;
      uVar2 = candidate_260c8();
      *(undefined2 *)((int)unaff_A6 + 0x86) = uVar2;
      *unaff_A6 = candidate_58e30;
    }
  }
  candidate_591fe();
  candidate_5929a();
  return;
}
