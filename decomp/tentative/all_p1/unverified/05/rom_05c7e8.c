/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05c7e8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5c7e8(void)

{
  short sVar1;
  short sVar2;
  undefined4 *unaff_A6;
  bool bVar3;
  undefined1 uVar4;

  candidate_5e720();
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0x40000;
  candidate_5e73a();
  candidate_4256();
  *unaff_A6 = 0x5c80e;
  uVar4 = 0;
  candidate_5e1e4();
  candidate_427a();
  candidate_5e26c();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_5cc3c;
    uVar4 = 0;
  }
  candidate_5e454();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_5cc3c;
    uVar4 = 0;
  }
  candidate_5e5ba();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_5cc3c;
    uVar4 = 0;
  }
  candidate_5e4f0();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_5cc3c;
    uVar4 = 0;
  }
  candidate_5e536();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_5cc3c;
    uVar4 = 0;
  }
  candidate_5e244();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_5cd08;
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
