/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07f940.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7f940(void)

{
  short sVar1;
  short extraout_D0u;
  ushort uVar3;
  int iVar2;
  short extraout_D1u;
  ushort uVar4;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  bool bVar5;

  func_0x000143e4();
  candidate_427a();
  candidate_7fc5e();
  if (!(bool)in_CF) {
    uVar3 = extraout_D0u - (short)((uint)unaff_A6[0xf] >> 0x10);
    if ((int)((uint)uVar3 << 0x10) < 0) {
      uVar3 = -uVar3;
    }
    uVar4 = extraout_D1u - ((short)((uint)unaff_A6[0x10] >> 0x10) + 0x10);
    if ((int)((uint)uVar4 << 0x10) < 0) {
      uVar4 = -uVar4;
    }
    if (((short)uVar3 < *(short *)(unaff_A6 + 0x1f)) &&
       ((short)uVar4 < *(short *)((int)unaff_A6 + 0x7e))) {
      *unaff_A6 = candidate_7f996;
    }
  }
  unaff_A6[0xd] = unaff_A6[0xd] - (unaff_A6[0xd] - *(int *)((int)unaff_A6 + 0x6e) >> 3);
  *(int *)((int)unaff_A6 + 0x72) =
       *(int *)((int)unaff_A6 + 0x72) - (*(int *)((int)unaff_A6 + 0x72) >> 3);
  *(char *)((int)unaff_A6 + 0x85) = *(char *)((int)unaff_A6 + 0x85) + '\x04';
  iVar2 = candidate_2e7c();
  unaff_A6[0xc] = *(int *)((int)unaff_A6 + 0x72) + iVar2;
  bVar5 = false;
  candidate_4c24();
  if ((bVar5) &&
     (*(undefined1 *)((int)unaff_A6 + 0x5a) = 0, (*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0)) {
    *unaff_A6 = candidate_7fa6c;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_7fa6c;
  }
  candidate_7fc46();
  sVar1 = *(short *)(unaff_A6 + 0x1e) + -1;
  *(short *)(unaff_A6 + 0x1e) = sVar1;
  if (sVar1 == 0) {
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8820;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8820;
  }
  return;
}
