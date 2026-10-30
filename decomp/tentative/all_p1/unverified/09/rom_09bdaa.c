/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09bdaa.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9bdaa(void)

{
  short sVar1;
  ushort uVar2;
  ushort uVar3;
  undefined4 *unaff_A6;
  undefined1 uVar4;
  bool bVar5;

  candidate_9cd36();
  candidate_4256();
  *unaff_A6 = 0x9bdc4;
  candidate_9cdf2();
  candidate_9ce20();
  candidate_9ccc6();
  candidate_427a();
  uVar3 = candidate_1ad7c();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    uVar2 = *(ushort *)(unaff_A6 + 0xf);
    if ((short)uVar3 >= (short)uVar2) {
      *unaff_A6 = candidate_9be1e;
    }
    bVar5 = (short)uVar3 < (short)uVar2 && uVar3 < uVar2;
  }
  else {
    bVar5 = uVar3 < *(ushort *)(unaff_A6 + 0xf);
    if ((short)uVar3 <= (short)*(ushort *)(unaff_A6 + 0xf)) {
      *unaff_A6 = candidate_9be1e;
      bVar5 = false;
    }
  }
  candidate_9d140();
  if (bVar5) {
    *unaff_A6 = candidate_9c23a;
  }
  sVar1 = *(short *)((int)unaff_A6 + 0x7a);
  uVar4 = sVar1 == 0;
  *(short *)((int)unaff_A6 + 0x7a) = sVar1 + -1;
  candidate_9cdb8();
  if ((bool)uVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar4 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
    candidate_9d470();
    if ((bool)uVar4) {
      *unaff_A6 = candidate_9c3d4;
    }
    return;
  }
  candidate_4e76();
  candidate_13f1e();
  *unaff_A6 = candidate_9b550;
  return;
}
