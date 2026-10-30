/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x035bf8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_35bf8(void)

{
  uint uVar1;
  short sVar3;
  int iVar2;
  undefined4 *unaff_A6;
  bool bVar4;

  *(undefined1 *)((int)unaff_A6 + 0x96) = 0;
  candidate_362ee();
  *(undefined4 *)((int)unaff_A6 + 0x7e) =
       *(undefined4 *)((short)(*(short *)((int)unaff_A6 + 0x86) * 4) + 0x35adc);
  candidate_35b1a();
  sVar3 = candidate_29f2();
  iVar2 = (int)(short)(*(short *)((int)unaff_A6 + 0x72) + sVar3 + -0xf0) << 8;
  unaff_A6[0xc] = iVar2;
  if (-1 < iVar2) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  *unaff_A6 = 0x35c42;
  unaff_A6[0xd] = *(undefined4 *)((int)unaff_A6 + 0x7e);
  uVar1 = *(uint *)((int)unaff_A6 + 0x7e);
  bVar4 = uVar1 < 0xffffec00;
  *(uint *)((int)unaff_A6 + 0x7e) = uVar1 + 0x1400;
  candidate_35af0();
  if (!bVar4) {
    candidate_1ac38();
    func_0x0003623e();
    candidate_364d2();
    return;
  }
  candidate_120f8();
  if (*(char *)((int)unaff_A6 + 0x9f) == '\a') {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  *unaff_A6 = candidate_360a8;
  return;
}
