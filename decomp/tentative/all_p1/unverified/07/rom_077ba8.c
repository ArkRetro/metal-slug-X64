/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x077ba8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_77ba8(void)

{
  int iVar1;
  byte bVar2;
  undefined4 *unaff_A6;
  bool bVar3;

  candidate_1abc8();
  candidate_427a();
  iVar1 = unaff_A6[3];
  bVar2 = *(byte *)(iVar1 + 0x9b);
  if (bVar2 == 2) {
    *(short *)(unaff_A6 + 10) = *(short *)((int)unaff_A6 + 0x86) + *(short *)(unaff_A6 + 10);
    *(undefined2 *)(iVar1 + 0x66) = 0xe0ed;
    *(undefined2 *)(iVar1 + 0x1e) = 0xe0ed;
    *unaff_A6 = candidate_1c71e;
  }
  bVar3 = bVar2 < 2;
  candidate_78d46();
  if (bVar3) {
    *unaff_A6 = 0x77e5c;
  }
  return;
}
