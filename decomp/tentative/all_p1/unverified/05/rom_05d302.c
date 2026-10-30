/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05d302.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5d302(void)

{
  ushort uVar1;
  short sVar2;
  short sVar3;
  ushort uVar4;
  undefined4 *unaff_A6;
  bool bVar5;
  undefined1 uVar6;

  candidate_5e720();
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0x40000;
  candidate_5e73a();
  unaff_A6[0x14] = 0x5e918;
  candidate_4256();
  *unaff_A6 = 0x5d330;
  candidate_5e1e4();
  candidate_427a();
  uVar4 = candidate_1ad7c();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    uVar1 = *(ushort *)(unaff_A6 + 0xf);
    if ((short)uVar4 >= (short)uVar1) {
      *unaff_A6 = candidate_5d390;
    }
    uVar6 = (short)uVar4 < (short)uVar1 && uVar4 < uVar1;
  }
  else {
    uVar6 = uVar4 < *(ushort *)(unaff_A6 + 0xf);
    if ((short)uVar4 <= (short)*(ushort *)(unaff_A6 + 0xf)) {
      *unaff_A6 = candidate_5d390;
      uVar6 = false;
    }
  }
  candidate_5e26c();
  if ((bool)uVar6) {
    *unaff_A6 = candidate_5d390;
    uVar6 = 0;
  }
  candidate_5e244();
  if ((bool)uVar6) {
    *unaff_A6 = candidate_5d27a;
  }
  candidate_5e6d2();
  sVar2 = *(short *)(unaff_A6 + 0x1f);
  sVar3 = sVar2 + -1;
  *(short *)(unaff_A6 + 0x1f) = sVar3;
  if (-1 >= sVar3) {
    *(undefined2 *)(unaff_A6 + 0x1f) = 0;
  }
  bVar5 = (*(byte *)((int)unaff_A6 + 0x5f) & 1) == 0;
  if (!bVar5) {
    *(undefined1 *)((int)unaff_A6 + 0x91) = 0x1e;
  }
  bVar5 = bVar5 && (-1 < sVar3 && sVar2 == 0);
  candidate_5e7a8();
  if (bVar5) {
    *unaff_A6 = candidate_5d732;
  }
  candidate_4c24();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  else {
    *unaff_A6 = candidate_5db18;
  }
  bVar5 = false;
  candidate_1945c();
  if (bVar5) {
    *unaff_A6 = candidate_5dc7a;
  }
  return;
}
