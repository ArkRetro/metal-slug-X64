/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05d596.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5d596(void)

{
  short sVar1;
  short sVar2;
  undefined4 *unaff_A6;
  bool bVar3;
  undefined1 uVar4;

  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0;
  unaff_A6[0x14] = 0x5e930;
  candidate_4256();
  *unaff_A6 = 0x5d5bc;
  uVar4 = 0;
  candidate_5e234();
  candidate_427a();
  candidate_5e67e();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_5d6e2;
    uVar4 = 0;
  }
  candidate_5e372();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_5d5f4;
    uVar4 = 0;
  }
  candidate_5e244();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_5d27a;
  }
  candidate_5e6d2();
  sVar1 = *(short *)(unaff_A6 + 0x1f);
  sVar2 = sVar1 + -1;
  *(short *)(unaff_A6 + 0x1f) = sVar2;
  if (-1 >= sVar2) {
    *(undefined2 *)(unaff_A6 + 0x1f) = 0;
  }
  bVar3 = (*(byte *)((int)unaff_A6 + 0x5f) & 1) == 0;
  if (!bVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x91) = 0x1e;
  }
  bVar3 = bVar3 && (-1 < sVar2 && sVar1 == 0);
  candidate_5e7a8();
  if (bVar3) {
    *unaff_A6 = candidate_5d732;
  }
  candidate_4c24();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  else {
    *unaff_A6 = candidate_5db18;
  }
  bVar3 = false;
  candidate_1945c();
  if (bVar3) {
    *unaff_A6 = candidate_5dc7a;
  }
  return;
}
